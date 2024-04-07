import json
import csv
import requests
import json
import time
import sqlite3
from tabulate import tabulate
import os
import pandas as pd
import threading
from datetime import datetime

global inhib, backgroundFetch, ThreadStop, ClockSignal
inhib = 0
backgroundFetch = 0
ThreadStop = 0
ClockSignal = 2

def clear_database():
    if os.path.exists("teams.db"):
        os.remove("teams.db")


def read_data_from_json(json_file):
    global inhib, backgroundFetch
    # Clear existing database
    clear_database()
    # os.system("clear")
    with open(json_file, "r") as file:
        teams_data = json.load(file)

    # Connect to SQLite database
    conn = sqlite3.connect("teams.db")
    cursor = conn.cursor()

    # Create table if it doesn't exist
    cursor.execute(
        """CREATE TABLE IF NOT EXISTS teams (
                        team_name TEXT,
                        team_email TEXT,
                        team_uni TEXT,
                        team_other_uni TEXT,
                        member_name TEXT,
                        member_email TEXT,
                        member_gender TEXT,
                        member_nic TEXT,
                        member_ieee TEXT,
                        member_pno TEXT,
                        member_food_pref TEXT,
                        member_git TEXT,
                        team_gist TEXT
                    )"""
    )

    for team in teams_data:
        team_name = team["team"]["name"]
        team_email = team["team"]["email"]
        team_uni = team["team"]["uni"]
        team_other_uni = team["team"]["otherUni"]
        team_gist = team["quizData"]

        for member in team["members"]:
            member_name = member["name"]
            member_email = member["email"]
            member_gender = member["gender"]
            member_nic = member["nic"]
            member_ieee = member["iee"]
            member_pno = member["no"]
            member_fp = member["food"]
            member_git = member["git"]

            # Insert data into the database
            cursor.execute(
                """INSERT INTO teams 
            (team_name,
             team_email, 
             team_uni, 
             team_other_uni, 
             member_name, 
             member_email, 
             member_gender,
             member_nic,
             member_ieee,
             member_pno,
             member_food_pref,
             member_git,
             team_gist)
                              VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)""",
                (
                    team_name,
                    team_email,
                    team_uni,
                    team_other_uni,
                    member_name,
                    member_email,
                    member_gender,
                    member_nic,
                    member_ieee,
                    member_pno,
                    member_fp,
                    member_git,
                    team_gist,
                ),
            )

            # Commit the changes
            conn.commit()
    query = """
            SELECT * FROM teams 
            """
    df = pd.read_sql_query(query, conn)
    df.to_excel("teams_data.xlsx", index=False)
    df.to_csv("teams_data.csv", index=False)
    # Query to fetch and count all unique teams
    cursor.execute("SELECT DISTINCT team_name,team_uni FROM teams ")
    teams = cursor.fetchall()

    cursor.execute("SELECT COUNT(team_name) FROM teams ")
    Totalteams = cursor.fetchall()

    # Query to fetch and count all unique universities
    cursor.execute(
        "SELECT team_uni, COUNT(*) as uni_count FROM teams GROUP BY team_uni"
    )
    universities = cursor.fetchall()

    # Display the counts as a table
    if backgroundFetch == 0:
        os.system("clear")
        print("- Fetching Live Data -")
        print("\nTeam Data\n")
        print("-" * 30)
        print(tabulate(teams, headers=["Team Name", "University"]))
        print("\nUniversity Count")
        print("-" * 30)
        print(
            tabulate(
                [(uni[0], int(uni[1]) / 4) for uni in universities],
                headers=["University", "Count"],
            )
        )
        print("-" * 30)
        print("Total Teams :", int(Totalteams[0][0]) / 4)
        current_datetime = datetime.now()
        print("Time:", current_datetime)
        if inhib == 1:
            os.system("./parser Override")
        # Close the connection
        conn.close()


def AutoFetchJson():
    response = requests.get(
        "ADD_GIST_LINK_HERE"
    )
    if response.status_code == 200:
        # Parse JSONL content
        lines = response.text.splitlines()
        data = [json.loads(line) for line in lines]

        # Save data to teams.json
        with open("teams.json", "w") as outfile:
            json.dump(data, outfile, indent=4)

        # print("Data saved to teams.json")
    else:
        print("Failed to fetch data from the URL !")
    read_data_from_json("teams.json")


def display():
    print("=============================================")
    print("DUOTHAN 4.0 JSON DATA PARSER                ")
    print("=============================================")
    print("1) AutoCheck Json data")
    print("2) Fetch Data in Background & Drop a SQL Shell")
    print("3) AutoCheck Json data | Low Power Mode")
    print("=============================================")


def backgroundFetchInit():
    global ThreadStop, ClockSignal
    ClockSignal = 2
    while ThreadStop != 0:
        AutoFetchJson()
        time.sleep(ClockSignal)


if __name__ == "__main__":
    print("Duothan4.0-JsonParser")
    inputComm = ""
    while True:
        display()
        checkIn = int(input("Enter Value :"))
        if checkIn == 1:
            ClockSignal = 2
            inhib = 0
            while True:
                AutoFetchJson()
                time.sleep(ClockSignal)
        elif checkIn == 2:
            conn = sqlite3.connect("teams.db")
            cursor = conn.cursor()
            backgroundFetch = 0
            bt = threading.Thread(target=backgroundFetchInit)
            bt.start()
            print("SQL Ready !")
            while inputComm != "exitThread":
                inputComm = input(">")
                cursor.execute(inputComm)
                rows = cursor.fetchall()
                for row in rows:
                    print(str(row).replace(",", "|"))
            cursor.close()
            conn.close()
            backgroundFetch = 1
            bt.stop()
        elif checkIn == 3:
            inhib = 0
            ClockSignal = 5
            while True:
                AutoFetchJson()
                time.sleep(ClockSignal)

    # print("Data saved successfully!")
