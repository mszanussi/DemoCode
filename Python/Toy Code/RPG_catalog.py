# Author : Milan Zanussi
# Date : December, 2015
# Project : RPG Catalog Tool
# Description : This is a tool I made for myself to catalog events for role-playing games.
# Some of the main intentions of the tool were the following :
#	1.) To provide myself a way of cataloging the events of an table-top role-playing game in files so campaigns could be remembered.
#	2.) To be lenient so that the tool could be used for any table-top RPG (Dungeons and Dragons, GURPS, etc.) at the discretion of the game master.
#	3.) To provide myself more diverse schemes for formulating dice rolls for deciding events.
#
# Additional Notes : This is very old code from the first semester of computer science class (CSCI 1170 at MTSU). I acknowledge that the code could be
# better modularized and the ontology of the program could be more well-designed. I included this code to demonstrate some prior experience with Python.
# Especially since I didn't feel like I had as much code that routinely used exception handling.

from random import *

def main():
    print("########## Welcome to the Role-Player's Catalog Tool! ##########")
    
    needHelp = ''
    while not(needHelp.lower() == 'yes' or needHelp.lower() == 'no'):
        needHelp = input('Do you know how to use this program? (Enter "Yes" or "No"): ')
        
        if needHelp.lower() == 'no':
            printInstructions()

        elif needHelp.lower() == 'yes':
            print("Welcome back!")
            
        else:
            print("""I'm sorry! I didn't understand that. Please use either "No" or "Yes".""")


    logError = True
    while logError:
        logError = False
        try:
            filename = input("Enter the name of the log file (all files are saved as .txt): ").rstrip('.txt')
            logFile = open(filename + '.txt', 'r')
        
        except FileNotFoundError:
            print("The file " + filename + '.txt' + ' could not be found.')
            createFile = '1'
            while not(createFile.lower() == 'yes' or createFile.lower() == 'no'):
                createFile = input("Would you like to create a log with this filename?: ")
                if createFile.lower() == 'yes':
                    logFile = open(filename + '.txt', 'a')
                    logError = False
                    newFile = True
                elif createFile.lower() == 'no':
                    print("Please double check your log's name. It can be found in the same folder as this program.")
                    logError = True
                else:
                    print("I'm sorry! I didn't understand that. Please use either 'No' or 'Yes'.")

        except:
            print("An unknown error has occurred.")
            logError = True

    refresher = 'maybe'
    while not(refresher.lower() == 'yes' or refresher.lower() == 'no') and newFile == False:
        refresher = input("Would you like to read what you have so far?: ")
        if refresher.lower() == 'no':
            logFile.close()
            logFile = open(filename + '.txt', 'a')
            
        elif refresher.lower() == 'yes':
            print(logFile.read())
            logFile.close()
            logFile = open(filename + '.txt', 'a')
            
        else:
            print("I'm sorry! I didn't understand that. Please use either 'No' or 'Yes'.")
    

    sessionRunning = True
    logFile.write("########## SESSION BEGINS ##########\n")
    while sessionRunning:
        event = input("Enter an event or description: ")
        if event == '-1':
            logFile.write("########## SESSION ENDS ##########\n")
            logFile.close()
            break
            
        dieCountError = True
        while dieCountError:
            try:
                diceCount = int(input("Enter a dice count ('0' for no dice): "))
            except ValueError:
                print("Please enter this field in numeric characters.")
            except:
                print("An unknown error has occurred.")
            else:
                dieCountError = False
                
        if diceCount >= 1:
            diceList = []
            for die in range(diceCount):
                dieSideError = True
                while dieSideError:
                    try:
                        dieSides = int(input("Enter side count for die " + str(die + 1) + ': '))
                    except ValueError:
                        print("Please enter this field in numeric characters.")
                    except:
                        print("An unknown error has occurred.")
                    else:
                        if dieSides < 2:
                            print("There's no such thing as " + str(dieSides) + "-sided dice!")
                        else:
                            dieSideError = False
                            diceList.append(dieSides)

            rollList = diceRoller(diceList)
            print("[" + event + "]\n")
            logFile.write("[" + event + "]\n\n")
            for roll in rollList:
                print("[Die " + str(roll[0]) + " rolled " + str(roll[1]) + ']\n')
                logFile.write("[Die " + str(roll[0]) + " rolled " + str(roll[1]) + ']\n\n')
        else:
            print("[" + event + "]\n")
            logFile.write("[" + event + "]\n\n")
        

def printInstructions():
    print("########## INSTRUCTIONS: ##########\n")
    print("First, you will need to enter a filename for your adventure.\nWhen naming a file, please only use alphanumeric characters, underscores(_), or dashes (-).\n")
    print("You will then be asked to enter the events and actions that happen in the game.\nAfter entering each event, you will be asked to enter the dice count for the event.\nEnter '0' if the event is an expositionary piece (stat rolling, scene describing, events beyond player control, etc.).\n")
    print("When a dice count is involved, you will be asked for the side count of each die.\nThe minimum side count is 2, and the sky is the limit for the maximum.\n")
    print("When the game is over for the session, Enter '-1' to properly close the session.\n")

def diceRoller(diceList):
    rollList = []
    for die in range(len(diceList)):
        rollList.append([(die + 1), randint(1,diceList[die])])
    return rollList 

main()
    
    
    
