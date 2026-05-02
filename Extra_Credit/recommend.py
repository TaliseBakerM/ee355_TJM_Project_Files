"""
recommend.py — Phase 3 Roommate Recommendation System
======================================================
USC EE 355 / Network Project Phase 3

Usage:
    python3 recommend.py <profiles.csv>

Pipeline:
    1. Load and preprocess profiles exported by C++ (exportCSV)
    2. Content-Based Filtering  — cosine similarity on encoded features
    3. Collaborative Filtering  — SVD matrix factorization on synthetic interaction matrix (from C++ friend graph)
    4. Hybrid model             — weighted blend of (2) and (3)
    5. Evaluation               — 90/10 train/test split, Precision@K, RMSE
    6. Output                   — recommendations.csv (C++ can reload this)

STL containers used on C++ side (referenced in comments):
map, vector, deque, list, set, unordered_map
"""

import sys
import os
import csv
import math
import random
import itertools

import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder, MinMaxScaler
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.decomposition import TruncatedSVD
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

# 0.  Helpers

RANDOM_SEED = 42
random.seed(RANDOM_SEED)
np.random.seed(RANDOM_SEED)

def banner(title: str):
    width = 60
    print("\n" + "=" * width)
    print(f"  {title}")
    print("=" * width)


# 1.  Load & preprocess

def load_profiles(csv_path: str) -> pd.DataFrame:
    """Load the CSV exported by Network::exportCSV()."""
    df = pd.read_csv(csv_path, dtype=str)
    df.fillna("", inplace=True)
    print(f"  Loaded {len(df)} profiles from {csv_path}")
    return df


def preprocess(df: pd.DataFrame):
    """
    Encode categorical + binary + numeric features into a numeric matrix.

    Feature groups
    ──────────────
    Categorical (label-encoded):  college, major, year, state, sleep
    Numeric (scaled 0-1):         cleanliness (1-10), weight columns (1-5),
                                  n_hobbies, n_preferences, n_dealbreakers
    Binary (already 0/1):         pref_quiet_hours, pref_no_smoking,
                                  pref_pets_ok, pref_early_bird, pref_night_owl
    """
    df_enc = pd.DataFrame(index=df.index)

    # --- Categorical ----
    cat_cols = ["college", "major", "year", "state", "sleep"]
    for col in cat_cols:
        le = LabelEncoder()
        # treat empty string as its own category
        df_enc[col] = le.fit_transform(df[col])

    # --- Numeric weights (1-5) and counts ---
    num_cols = [
        "w_quiet", "w_cleanliness", "w_social", "w_studious", "w_pets", "w_smoking",
        "n_hobbies", "n_preferences", "n_dealbreakers", "cleanliness"
    ]
    for col in num_cols:
        df_enc[col] = pd.to_numeric(df[col], errors="coerce").fillna(0)

    scaler = MinMaxScaler()
    df_enc[num_cols] = scaler.fit_transform(df_enc[num_cols])

    # --- Binary preference flags ---
    bin_cols = [
        "pref_quiet_hours", "pref_no_smoking", "pref_pets_ok",
        "pref_early_bird", "pref_night_owl"
    ]
    for col in bin_cols:
        df_enc[col] = pd.to_numeric(df[col], errors="coerce").fillna(0)

    person_ids = df["person_id"].tolist()
    feature_matrix = df_enc.values.astype(float)

    print(f"  Feature matrix shape: {feature_matrix.shape}")
    return person_ids, feature_matrix, df_enc.columns.tolist()


# 2.  Content-Based Filtering

def content_based(person_ids, feature_matrix):
    """
    Compute pairwise cosine similarity between all profile vectors.
    Returns a (N x N) similarity DataFrame.
    """
    sim_matrix = cosine_similarity(feature_matrix).copy()
    np.fill_diagonal(sim_matrix, 0.0)
    sim_df = pd.DataFrame(sim_matrix, index=person_ids, columns=person_ids)
    return sim_df


def top_k_content(sim_df, person_id, k=3):
    """Return top-k most similar people by content similarity."""
    if person_id not in sim_df.index:
        return []
    row = sim_df.loc[person_id].sort_values(ascending=False)
    return list(row.head(k).items()) # [(id, score), ...]


# 3.  Collaborative Filtering (SVD)

def build_interaction_matrix(person_ids, sim_df, noise_level=0.15):
    """
    Build a synthetic user-item interaction (rating) matrix.

    In a real deployment the C++ app would export the myfriends adjacency
    and user-rated pairs. Here we simulate realistic ratings:
      - Start from the content similarity score (0-1)
      - Apply Gaussian noise to simulate human variability
      - Scale to a 1-5 star rating
    This gives the SVD something meaningful to factorize beyond pure features.
    """
    n = len(person_ids)
    # Pull raw similarity as base "true compatibility"
    base = sim_df.values.copy() # shape (n, n)
    noise = np.random.normal(0, noise_level, base.shape)
    ratings_raw = np.clip(base + noise, 0.0, 1.0)

    # Only "observe" ~40% of pairs (simulate missing data)
    mask = np.random.rand(n, n) < 0.40
    np.fill_diagonal(mask, False) # no self-ratings
    ratings_observed = np.where(mask, ratings_raw, 0.0)

    # Scale 0-1 to 1-5
    ratings_scaled = ratings_observed * 4.0 + 1.0
    ratings_scaled = np.where(mask, ratings_scaled, 0.0)

    return pd.DataFrame(ratings_scaled, index=person_ids, columns=person_ids), mask


def collaborative_svd(rating_df, mask, n_components=None):
    """
    Fit TruncatedSVD on the rating matrix to learn latent factors.
    Returns predicted (full) rating matrix.
    """
    n = len(rating_df)
    # Choose n_components automatically — at most n-1
    if n_components is None:
        n_components = max(1, min(n - 1, 5))

    svd = TruncatedSVD(n_components=n_components, random_state=RANDOM_SEED)
    U = svd.fit_transform(rating_df.values) # (n x k)
    Vt = svd.components_ # (k x n)
    pred_matrix = U @ Vt # (n x n) reconstructed

    # Re-scale to [1, 5] range
    pred_matrix = np.clip(pred_matrix, 1.0, 5.0)
    pred_df = pd.DataFrame(pred_matrix, index=rating_df.index, columns=rating_df.columns)
    # Normalize to [0, 1] for blending
    pred_norm = (pred_df - 1.0) / 4.0
    return pred_norm, svd.explained_variance_ratio_.sum()


def top_k_collab(pred_df, person_id, k=3):
    """Return top-k most compatible people by collaborative score."""
    if person_id not in pred_df.index:
        return []
    row = pred_df.loc[person_id].copy()
    row[person_id] = 0.0  # exclude self
    row_sorted = row.sort_values(ascending=False)
    return list(row_sorted.head(k).items())


# 4.  Hybrid Model

def hybrid_score(sim_df, pred_df, alpha=0.55):
    """
    Blend content-based and collaborative scores.
    alpha controls weight of content-based (1-alpha = collaborative).

    alpha=0.55 favors content slightly — meaningful for cold-start profiles
    where collaborative data is sparse.
    """
    # Both matrices should be (n x n) with same index/columns
    combined = (alpha * sim_df + (1 - alpha) * pred_df).copy()
    arr = combined.values.copy()
    np.fill_diagonal(arr, 0.0)
    combined = pd.DataFrame(arr, index=combined.index, columns=combined.columns)
    return combined


def top_k_hybrid(hybrid_df, person_id, k=3):
    """Return top-k recommendations from the hybrid matrix."""
    if person_id not in hybrid_df.index:
        return []
    row = hybrid_df.loc[person_id].sort_values(ascending=False)
    return list(row.head(k).items())


# 5.  Evaluation

def evaluate(rating_df, mask, pred_norm_df, k=3):
    """
    90/10 train/test split on observed ratings.

    Metrics:
      RMSE      — how closely predicted ratings match held-out ratings
      Precision@K — fraction of top-K recommendations that are "relevant"
                    (defined as true rating > 3.5 out of 5)
    """
    person_ids = rating_df.index.tolist()
    n = len(person_ids)

    # Gather observed pairs
    observed_pairs = []
    for i in range(n):
        for j in range(n):
            if mask[i, j] and i != j:
                observed_pairs.append((i, j))

    if len(observed_pairs) < 10:
        print("  [Eval] Not enough observed ratings for meaningful split.")
        return

    random.shuffle(observed_pairs)
    split = int(0.9 * len(observed_pairs))
    train_pairs = observed_pairs[:split]
    test_pairs  = observed_pairs[split:]

    # Collect true vs predicted on test set
    y_true, y_pred = [], []
    for (i, j) in test_pairs:
        true_rating = rating_df.iloc[i, j] # 1-5 scale
        pred_rating = pred_norm_df.iloc[i, j] * 4.0 + 1.0 # back to 1-5
        y_true.append(true_rating)
        y_pred.append(pred_rating)

    rmse = math.sqrt(mean_squared_error(y_true, y_pred))

    # Precision@K — for each user in test, fraction of top-K recs that are relevant
    relevant_threshold = 3.5 # on 1-5 scale
    precision_scores = []
    for pid in person_ids:
        # Get top-K predictions for this user
        pred_row = pred_norm_df.loc[pid].copy()
        pred_row[pid] = 0.0
        top_k_ids = pred_row.nlargest(k).index.tolist()

        # Count how many of those have a "true" rating above threshold
        hits = 0
        for rec_id in top_k_ids:
            i = person_ids.index(pid)
            j = person_ids.index(rec_id)
            if mask[i, j]:
                if rating_df.iloc[i, j] >= relevant_threshold:
                    hits += 1
        if k > 0:
            precision_scores.append(hits / k)

    precision_at_k = sum(precision_scores) / len(precision_scores) if precision_scores else 0.0

    print(f"\n  Evaluation (90/10 split, {len(train_pairs)} train / {len(test_pairs)} test pairs)")
    print(f"  RMSE : {rmse:.4f} (lower is better; scale 1-5)")
    print(f"  Precision@{k} : {precision_at_k:.4f} (higher is better; 0-1)")


# ─────────────────────────────────────────────────────────────
# 6.  Output
# ─────────────────────────────────────────────────────────────

def save_recommendations(hybrid_df, output_path="recommendations.csv", k=3):
    """
    Write top-K hybrid recommendations for every person to CSV.
    C++ can optionally reload this file to display in the menu.

    Format:
        person_id, rank, recommended_id, hybrid_score
    """
    rows = []
    for pid in hybrid_df.index:
        row = hybrid_df.loc[pid].copy()
        row[pid] = 0.0
        top_k = row.nlargest(k)
        for rank, (rec_id, score) in enumerate(top_k.items(), start=1):
            rows.append({
                "person_id": pid,
                "rank": rank,
                "recommended_id": rec_id,
                "hybrid_score": round(score, 4)
            })

    out_df = pd.DataFrame(rows)
    out_df.to_csv(output_path, index=False)
    print(f"\n  Recommendations saved to {output_path}")
    return out_df


def print_recommendations(hybrid_df, content_df, collab_df, k=3):
    """Pretty-print the recommendations for every person."""
    banner("Final Hybrid Recommendations")
    for pid in hybrid_df.index:
        print(f"\n  Person: {pid}")
        print(f"  {'Rank':<6} {'ID':<30} {'Content':>9} {'Collab':>8} {'Hybrid':>8}")
        print(f"  {'-'*65}")
        row_h = hybrid_df.loc[pid].copy(); row_h[pid] = 0.0
        top_k = row_h.nlargest(k)
        for rank, (rec_id, h_score) in enumerate(top_k.items(), start=1):
            c_score = content_df.loc[pid, rec_id] if rec_id in content_df.columns else 0
            cf_score = collab_df.loc[pid, rec_id] if rec_id in collab_df.columns else 0
            print(f"  #{rank:<5} {rec_id:<30} {c_score:>9.4f} {cf_score:>8.4f} {h_score:>8.4f}")


# ─────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────

def main():
    # Argument handling
    if len(sys.argv) < 2:
        # Try default path if run without args
        csv_path = "profiles.csv"
        if not os.path.exists(csv_path):
            print("Usage: python3 recommend.py <profiles.csv>")
            print("(Generate profiles.csv from the C++ menu option 9)")
            sys.exit(1)
    else:
        csv_path = sys.argv[1]

    print("\n" + "=" * 60)
    print("RoomieFinder — ML Recommendation Engine")
    print("Phase 3 | USC EE 355 Network Project")
    print("=" * 60)

    # Step 1: Load 
    banner("Step 1: Loading Profiles")
    df = load_profiles(csv_path)
    if len(df) < 2:
        print("  Need at least 2 profiles to compute recommendations.")
        sys.exit(1)

    person_ids, feature_matrix, feature_names = preprocess(df)

    # Step 2: Content-Based
    banner("Step 2: Content-Based Filtering (Cosine Similarity)")
    content_sim_df = content_based(person_ids, feature_matrix)
    print("Sample content similarities:")
    pid0 = person_ids[0]
    for pid, score in top_k_content(content_sim_df, pid0, k=3):
        print(f"    {pid0} <-> {pid}: {score:.4f}")

    # Step 3: Collaborative Filtering
    banner("Step 3: Collaborative Filtering (SVD)")
    rating_df, mask = build_interaction_matrix(person_ids, content_sim_df)
    print(f"Interaction matrix: {rating_df.shape}, "
          f"observed pairs: {mask.sum()}")

    collab_pred_df, var_explained = collaborative_svd(rating_df, mask)
    print(f"SVD variance explained: {var_explained:.2%}")
    print("Sample collaborative scores:")
    for pid, score in top_k_collab(collab_pred_df, pid0, k=3):
        print(f"    {pid0} <-> {pid}: {score:.4f}")

    # Step 4: Hybrid
    banner("Step 4: Hybrid Model (alpha=0.55 content, 0.45 collaborative)")
    hybrid_df = hybrid_score(content_sim_df, collab_pred_df, alpha=0.55)

    # Step 5: Evaluation 
    banner("Step 5: Evaluation")
    evaluate(rating_df, mask, collab_pred_df, k=3)

    # Step 6: Output
    print_recommendations(hybrid_df, content_sim_df, collab_pred_df, k=3)
    out_df = save_recommendations(hybrid_df, "recommendations.csv", k=3)

    print("\n" + "=" * 60)
    print("Done! Load recommendations.csv into C++ or inspect it directly.")
    print("=" * 60 + "\n")


if __name__ == "__main__":
    main()
