

#include <iostream>
#include <string>
#include <limits>
#include <cctype>

auto clearBuf() -> void {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

const std::string strs[] {
	"Flmpx", "hy", "HHMM", "hhmm", "Apple", "Double", "fdald", "djalsds", "kdasld",
	"ksdjald", "oiwes0f90", "ksajiweiw23", "aaaaaa", "bbb", "666"
};

const std::size_t num {sizeof(strs) / sizeof(std::string)};

auto main() -> int
{
	std::string target {};
	
	std::srand(time(NULL));
	
	std::cout << "Will you play a word game? <y/n> ";
	char choice {'\0'};
	std::cin >> choice;
	clearBuf();
	
	while (std::tolower(choice) == 'y') {
		target = strs[std::rand() % num];
		std::string badchars {};
		std::string attemp(target.size(), '-');
		
		int guesses = 26;
		std::cout << "You have " << guesses << " chance!\n";
		std::cout << "Target string have " << target.size() << " characters\n\n";
		
		while (guesses > 0 && attemp != target) {
			std::cout << "Guess a letter: ";
			char guess {'\0'};
			std::cin >> guess;
			clearBuf();
			
			/* 如果已经尝试过了就不要计入 */
			if (badchars.find(guess) != std::string::npos || 
				attemp.find(guess) != std::string::npos) {
				std::cout << "You already guessed that. Try again!\n";
				continue;
			}
			
			guesses--;
			
			std::size_t loc {std::string::npos};
			if ((loc = target.find(guess)) != std::string::npos) {
				std::cout << ">>> Good Guess <<<\n";
				do {
					attemp[loc] = guess;
				} while ((loc = target.find(guess, loc + 1)) != std::string::npos);
				
			} else {
				std::cout << ">>> Bad Guess <<<\n";
				badchars += guess;
			}
			
			std::cout << "Your word: " << attemp << "\n";
			std::cout << "Bad Choices: " << badchars << "\n";
			std::cout << "\n";
		}
		
		if (target == attemp) {
			std::cout << "You're right~~~\n\n";
		} else {
			std::cout << "Sorry the target word is " << target << "\n\n";
		}
		
		std::cout << "Will you play another? <y/n> ";
		std::cin >> choice;
		clearBuf();
	}
	
	std::cout << "Bye!!!\n";
	
	return 0;
}