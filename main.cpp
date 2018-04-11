/*
 * main.cpp
 *
 *  Created on: Apr 6, 2018
 *      Author: Luke Clements
 */

#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

int searchAndReplace(string & doc, string toReplace, string replaceWith);

int main()
{
	string toReplace;
	string replaceWith;
	//"Document" from: https://en.wikipedia.org/wiki/RSA_(cryptosystem)
	string document = "RSA (Rivest–Shamir–Adleman) is one of the first public-key "
			"cryptosystems and is widely used for secure data transmission. In such a "
			"cryptosystem, the encryption key is public and it is different from the "
			"decryption key which is kept secret (private). In RSA, this asymmetry is "
			"based on the practical difficulty of the factorization of the product of "
			"two large prime numbers, the 'factoring problem'. The acronym RSA is made "
			"of the initial letters of the surnames of Ron Rivest, Adi Shamir, and "
			"Leonard Adleman, who first publicly described the algorithm in 1978. "
			"Clifford Cocks, an English mathematician working for the British intelligence "
			"agency Government Communications Headquarters (GCHQ), had developed an "
			"equivalent system in 1973, but this was not declassified until 1997.[1] "
			"A user of RSA creates and then publishes a public key based on two large "
			"prime numbers, along with an auxiliary value. The prime numbers must be "
			"kept secret. Anyone can use the public key to encrypt a message, but with "
			"currently published methods, and if the public key is large enough, only "
			"someone with knowledge of the prime numbers can decode the message feasibly.[2] "
			"Breaking RSA encryption is known as the RSA problem. Whether it is as "
			"difficult as the factoring problem remains an open question. RSA is a "
			"relatively slow algorithm, and because of this, it is less commonly used "
			"to directly encrypt user data. More often, RSA passes encrypted shared keys "
			"for symmetric key cryptography which in turn can perform bulk "
			"encryption-decryption operations at much higher speed. \nHistory: "
			"The idea of an asymmetric public-private key cryptosystem is attributed to "
			"Whitfield Diffie and Martin Hellman, who published this concept in 1976. "
			"They also introduced digital signatures and attempted to apply number theory. "
			"Their formulation used a shared-secret-key created from exponentiation of "
			"some number, modulo a prime number. However, they left open the problem of "
			"realizing a one-way function, possibly because the difficulty of factoring "
			"was not well-studied at the time.[3] Ron Rivest, Adi Shamir, and "
			"Leonard Adleman at the Massachusetts Institute of Technology made several "
			"attempts, over the course of a year, to create a one-way function that was "
			"hard to invert. Rivest and Shamir, as computer scientists, proposed many "
			"potential functions, while Adleman, as a mathematician, was responsible for "
			"finding their weaknesses. They tried many approaches including 'knapsack-based' "
			"and 'permutation polynomials'. For a time, they thought what they wanted to "
			"achieve was impossible due to contradictory requirements.[4] In April 1977, "
			"they spent Passover at the house of a student and drank a good deal of "
			"Manischewitz wine before returning to their homes at around midnight.[5] "
			"Rivest, unable to sleep, lay on the couch with a math textbook and started "
			"thinking about their one-way function. He spent the rest of the night "
			"formalizing his idea, and he had much of the paper ready by daybreak. "
			"The algorithm is now known as RSA – the initials of their surnames in same "
			"order as their paper.[6] Clifford Cocks, an English mathematician working "
			"for the British intelligence agency Government Communications Headquarters "
			"(GCHQ), described an equivalent system in an internal document in 1973.[7] "
			"However, given the relatively expensive computers needed to implement it at "
			"the time, RSA was considered to be mostly a curiosity and, as far as is "
			"publicly known, was never deployed. His discovery, however, was not revealed "
			"until 1997 due to its top-secret classification. Kid-RSA (KRSA) is a "
			"simplified public-key cipher published in 1997, designed for educational "
			"purposes. Some people feel that learning Kid-RSA gives insight into RSA and "
			"other public-key ciphers, analogous to simplified DES.[8][9][10][11][12]";
	long childPID;
	int entriesReplaced;

	//gets user input
	cout << "Which word would you like to replace? ";
	cin >> toReplace;
	if(toReplace != "!wq")
	{
		cout << "Which word would you like to replace " << toReplace << " with? ";
		cin >> replaceWith;
	}

	while(toReplace != "!wq" && replaceWith != "!wq")
	{
		childPID = fork();

		//only a child can enter here
		if(childPID == 0)
		{
			entriesReplaced = searchAndReplace(document, toReplace, replaceWith);
			cout << "Child " << getpid() << " found and replaced "
				 << entriesReplaced << " entries of " << toReplace << " with " << replaceWith << endl;
			cout << document << endl;
			break;
		}
		//only the parent can enter here
		else
		{
			//waits for the child process to finish
			waitpid(childPID, NULL, 0);

			//gets user input
			cout << "Which word would you like to replace? ";
			cin >> toReplace;
			if(toReplace != "!wq")
			{
				cout << "Which word would you like to replace " << toReplace << " with? ";
				cin >> replaceWith;
			}
		}
	}

	return 0;
}

//searches and replaces for the specified words in a document that will be
//	changed in the calling function and returns the amount of times the
//	searched word was replaced
int searchAndReplace(string & document, string toReplace, string replaceWith)
{
	int countOfReplacements = 0;
	size_t positionFoundSubstring;

	//will look through the document until a toReplace string is found in the document. Will infinite loop and need
	//	termination elsewhere (terminal) if there are no substrings that match toReplace
	while(countOfReplacements == 0)
	{
		//will continue to look through document for the toReplace string until all are found
		do
		{
			positionFoundSubstring = document.find(toReplace);
			if(positionFoundSubstring != string::npos)
			{
				document.replace(positionFoundSubstring, toReplace.size(), replaceWith);
				countOfReplacements++;
			}
		}while(positionFoundSubstring != string::npos);
		if(countOfReplacements == 0)
		{
			cout << ". " << getpid() << endl;
		}
	}
	return countOfReplacements;
}
