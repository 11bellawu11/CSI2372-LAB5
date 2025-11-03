
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "myFile.h"
#include "Card.h"
#include "CardsSet.h"
#include "Player.h"

int main() {
	CardsSet packet;
	Player you(packet, false);
	Player me(packet, true);
	char answer[3];
	bool continuous = true;
	cout << "Hello! " << endl;
	while (continuous)
	{
		cout << "A new game? " << endl;
		cin >> answer;
		continuous = answer[0] == 'y';
		if (continuous)
		{
			packet.novSet();
			packet.shuffle();
			packet.take();
			int p1 = you.play();
			if (p1 > 21) {
				cout << "You lost! " << endl;
			}
			else if (p1 == 21) {
				cout << "You won! " << endl;
			}
			else // the computer must play
			{
				int p2 = me.play();
				if (p2 <= 21 && p2 >= p1) {
					cout << "You lost! " << endl;
				}
				else {
					cout << "You won! " << endl;
				}
			}
		}
	}
	return 0;
}

void Card::write() {
	string suites[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
	switch (val) {
		case 11:
			cout << "Jack of " << suites[col] << endl;
			break;
		case 12:
			cout << "Queen of " << suites[col] << endl;
			break;
		case 13:
			cout << "King of " << suites[col] << endl;
			break;
		case 1:
			cout << "Ace of " << suites[col] << endl;
			break;
		default:
			cout << val << " of " << suites[col] << endl;
			break;
	}
}

/**
 * create new set of 52 cards
 */
void CardsSet::novSet() {
	for (int i = 1; i <= 13; i++) {
		for (int c = 0; c < 4; c++) {
			set[number++] = Card(static_cast<color>(c), i);
		}
	}
}

/**
 * shuffles cards
 */
void CardsSet::shuffle() {
	srand(time(0));
	for (int i = number - 1; i > 0; i--) { 
        int j = rand() % (i + 1); 
        swap(set[i], set[j]); 
	}
}

/**
 * take a card from the set
 */
Card CardsSet::take() {
	if (number > 0) {
		return set[--number]; // take from top (0 = bottom)
	}
	cout << "No more cards in set" << endl;
	return Card();
}

/**
 * put a card in the set
 */
void CardsSet::put(Card k) {
	if (number < 52) {
		set[number++] = k;
	}
}

/**
 * find the card at position 'no'
 */
Card CardsSet::lookIn(int no) {
	if (no >= 1 && no <= number) {
        return set[number - no]; // 1 = top card = total number of cards - 1
	}
	cout << "Out of range" << endl; 
	return Card();
}


int Player::countPoints() {
    int sum = 0;
    int aces = 0;

    for (int i = 1; i <= inHand.numCards(); i++) {
        Card c = inHand.lookIn(i);
        int v = c.value();
        if (v == 1)
            aces++;
        else if (v > 10)
            v = 10; // J, Q, K = 10
        sum += v;
    }

    // Handle Aces (1 or 14)
    while (aces > 0) {
        if (sum + 14 <= 21)
            sum += 14;
        else
            sum += 1;
        aces--;
    }

    return sum;
}


/**
 * Play: draws cards and returns total score.
 * - User decides when to stop.
 * - Computer stops when >= 17.
 */
int Player::play() {
    char answer = 'y';
    int score = 0;
    inHand.empty(); // clear previous hand before each round

    if (computer) {
        cout << "\nComputer's turn..." << endl;
        do {
            Card c = packet.take();
            inHand.put(c);
            cout << "Computer gets card: ";
            c.write();
            cout << endl;
            score = countPoints();
        } while (score < 17);
        cout << "Computer's score: " << score << endl;
        return score;
    } 
    else {
        // Human player
        while (answer == 'y' || answer == 'Y') {
            Card c = packet.take();
            inHand.put(c);

            cout << "You get card: ";
            c.write();
            cout << endl;

            score = countPoints();
            cout << "Your score is " << score << " points" << endl;

            if (score > 21)
                return score; // lost

            cout << "Any additional Card? (y/n): ";
            cin >> answer;
        }
        return score;
    }
}