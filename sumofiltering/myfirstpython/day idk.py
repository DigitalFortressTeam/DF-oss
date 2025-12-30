"""
rock paper scissors time
"""
import random
import time
play_choice = ["yes", "no"]
play_again = True
player_choice_to_play_again = None
while play_again:
    choices = ["rock", "paper", "scissors"]
    computer = random.choice(choices)
    player = None
    while player not in choices:
        player = input("choose between: rock, paper, or scissors: ").lower()
    time.sleep(0.25)
    print(f"you played: {player}, while the computer played: {computer}.")
    time.sleep(0.25)
    if player == computer:
        print("it is a draw!")
    else:
        if player == "rock":
            if computer == "paper":
                print("you lost!")
            elif computer == "scissors":
                print("you won!")
        if player == "paper":
            if computer == "rock":
                print("you won!")
            elif computer == "scissors":
                print("you lost!")
        if player == "scissors":
            if computer == "paper":
                print("you won!")
            elif computer == "rock":
                print("you lost!")
    while player_choice_to_play_again not in play_choice:
        player_choice_to_play_again = input("wanna play again(yes/no): ").lower()
    time.sleep(0.25)
    if player_choice_to_play_again == "no":
        play_again = False
    elif player_choice_to_play_again == "yes":
        player_choice_to_play_again = None
