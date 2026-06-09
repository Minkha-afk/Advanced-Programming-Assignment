# Assignment 7

## Question

Develop an activity log analyzer in python:

You are given a list of activity records:
{
    "user": str,              //roll numbers of students
    "action": str,          //Online activities of students such as - apps, websites visited etc.
    "duration": float   //screen time for each activity
}

You must:

1. Store data efficiently using python built-in containers.

2. Implement the following:
def total_time_per_user(logs: list[dict]) -> dict[str, float]
def most_active_users(logs: list[dict], k: int) -> list[str]
def unique_actions(logs: list[dict]) -> set[str]

3.
a. Use dict, set, and list
b. Use comprehensions where appropriate
c. Use sorted() with key
d. Avoid explicit loops where possible
e. Use typing annotations
f. Use defaultdict optionally
f. Use reduce() to compute total activity time.

4. Perform complexity analysis:
a. Time complexity for computing top K users
b. Space complexity of storing intermediate results
