"""
Assignment 7: Activity Log Analyzer
"""

from collections import defaultdict
from functools import reduce
from typing import Any

# ── Sample Data ────────────────────────────────────────────────────────────────

logs: list[dict[str, Any]] = [
    {"user": "CS101", "action": "YouTube",     "duration": 45.0},
    {"user": "CS102", "action": "Instagram",   "duration": 30.0},
    {"user": "CS101", "action": "Google Docs", "duration": 60.0},
    {"user": "CS103", "action": "YouTube",     "duration": 90.0},
    {"user": "CS102", "action": "Zoom",        "duration": 50.0},
    {"user": "CS101", "action": "Zoom",        "duration": 40.0},
    {"user": "CS104", "action": "Netflix",     "duration": 120.0},
    {"user": "CS103", "action": "Google Docs", "duration": 35.0},
    {"user": "CS104", "action": "YouTube",     "duration": 80.0},
    {"user": "CS102", "action": "Netflix",     "duration": 70.0},
]


# ── 1. Total time per user using reduce() ──────────────────────────────────────

def total_time_per_user(logs: list[dict]) -> dict[str, float]:
    def add_duration(acc: defaultdict, record: dict) -> defaultdict:
        acc[record["user"]] += record["duration"]
        print(f"Adding {record['duration']} min for {record['user']} (Total: {acc[record['user']]} min)")
        return acc

    return dict(reduce(add_duration, logs, defaultdict(float)))


# ── 2. Top K most active users ─────────────────────────────────────────────────

def most_active_users(logs: list[dict], k: int) -> list[str]:
    totals = total_time_per_user(logs)
    ranked = sorted(totals, key=lambda user: totals[user], reverse=True)
    return ranked[:k]


# ── 3. Unique actions using a set ──────────────────────────────────────────────

def unique_actions(logs: list[dict]) -> set[str]:
    return {record["action"] for record in logs}


# ── Run & Print Results ────────────────────────────────────────────────────────

print("=== Total Time Per User ===")
totals = total_time_per_user(logs)
for user, mins in totals.items():
    print(f"  {user}: {mins} min")

print("\n=== Top 3 Most Active Users ===")
top3 = most_active_users(logs, k=3)
for i, user in enumerate(top3, start=1):
    print(f"  #{i} {user} — {totals[user]} min")

print("\n=== Unique Actions ===")
actions = unique_actions(logs)
print(f"  {actions}")

print("\n=== Complexity Analysis ===")
print("  total_time_per_user -> Time: O(n),            Space: O(u)")
print("  most_active_users   -> Time: O(n + u·log u),  Space: O(u)")
print("  unique_actions      -> Time: O(n),            Space: O(a)")
print("\n  n=log entries, u=unique users, a=unique actions")