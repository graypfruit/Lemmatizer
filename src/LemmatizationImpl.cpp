// This stub implementation returns each input word unchanged and
// does no input errors handling

#include "LemmatizationImpl.h"

using namespace std;

namespace lemmatizer {

	static const string vowels("���������");
	static const string consonants("�������������������");
	static const regex perfectgerund = regex("(.*)(������|������|������|������|����|����|����|����|��|��|��|��)$");
	static const regex perfectgerund1M = regex("(.*)(������|������|����|����|��|��)$");
	static const regex perfectgerund1 = regex("(�����|���|�)$");
	static const regex perfectgerund2 = regex("(������|������|����|����|��|��)$");
	static const regex reflexive = regex("(��|��)$");
	static const regex reflexiveM = regex("(.*)(��|��)$");
	static const regex adjective = regex("(���|���|���|���|���|���|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��)$");
	static const regex adjectiveM = regex("(.*)(���|���|���|���|���|���|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��)$");
	static const regex participle = regex("(.*)(���|���|������|���|���|���|��|���|���|���|���|��)$");
	static const regex participle1M = regex("(.*)(���|���|���|���|��|���|���|���|���|��)$");
	static const regex participle1 = regex("(��|��|��|��|�)$");
	static const regex participle2 = regex("(���|���|���)$");
	static const regex verb = regex("(.*)(����|����|����|����|���|���|���|���|���|���|���|���|���|���|��|��|��|����|����|����|����|���|\
���|���|���|���|���|���|���|���|���|��|��|��|����|����|���|���|���|���|���|���|���|���|���|���|���|���|���|���|���|��|��|��|��|��|\
��|��|��|��|��|��|�)$");
	static const regex verb1M = regex("(.*)(����|����|����|����|���|���|���|���|���|���|���|���|���|���|��|��|��|����|����|����|����|\
���|���|���|���|���|���|���|���|���|���|��|��|��)$");
	static const regex verb1 = regex("(���|���|���|���|��|��|��|��|��|��|��|��|��|��|�|�|�)$");
	static const regex verb2 = regex("(����|����|���|���|���|���|���|���|���|���|���|���|���|���|���|���|���|��|��|��|��|��|��|��|��|\
��|��|��|�)$");
	static const regex nounM = regex("(.*)(����|���|���|���|���|���|���|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|�|�|\
�|�|�|�|�|�|�|�)$");
	static const regex noun = regex("(����|���|���|���|���|���|���|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|��|�|�|�|�|�|\
�|�|�|�|�)$");
	static const regex i = regex("�$");
	static const regex iM = regex("(.*)(�)$");
	static const regex pM = regex("(.*)�$");
	static const regex p = regex("�$");
	static const regex nnM = regex("(.*)(��)$");
	static const regex nn = regex("(�)$");
	static const regex derivationalM = regex("(.*)(����|���)$");
	static const regex derivational = regex("(����|���)$");
	static const regex superlativeM = regex("(.*)(����|���)$");
	static const regex superlative = regex("(����|���)$");
	
	LemmatizationImpl::LemmatizationImpl(std::istream &input_stream)
        : input_stream_(input_stream), last_lemma_() {}

    const std::string &LemmatizationImpl::last_lemma() const {
        return last_lemma_;
    }

    const std::string *LemmatizationImpl::next_lemma() {
        std::string input_str;
        if (input_stream_ >> input_str) {
			findStem(input_str);
            last_lemma_ = input_str;
            return &last_lemma_;
        } else {
            return nullptr;
        }
    }

	const string LemmatizationImpl::get_stem() const
	{
		return stem_;
	}

	void LemmatizationImpl::findStem(string inputWord)
	{
		transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower);
		replace(inputWord.begin(), inputWord.end(), '�', '�');

		string stem = inputWord;
		string RV = findRV(stem);
		string R1 = findR1(RV);
		string R2 = findR2(R1);
		string endings[4];

		if (regex_match(RV, perfectgerund))
		{
			if (regex_match(RV, perfectgerund1M))
				cutEnding(stem, RV, R2, endings[0], perfectgerund1);
			else
				cutEnding(stem, RV, R2, endings[0], perfectgerund2);
		}
		else
		{
			if (regex_match(RV, reflexiveM))
				cutEnding(stem, RV, R2, endings[0], reflexive);
			if (regex_match(RV, adjectiveM))
			{
				cutEnding(stem, RV, R2, endings[0], adjective);
				if (regex_match(RV, participle))
				{
					if (regex_match(RV, participle1M))
						cutEnding(stem, RV, R2, endings[0], participle1);
					else
						cutEnding(stem, RV, R2, endings[0], participle2);
				}
			}
			else if (regex_match(RV, participle))
			{
				if (regex_match(RV, participle1M))
					cutEnding(stem, RV, R2, endings[0], participle1);
				else
					cutEnding(stem, RV, R2, endings[0], participle2);
			}
			else if (regex_match(RV, verb))
			{
				if (regex_match(RV, verb1M))
					cutEnding(stem, RV, R2, endings[0], verb1);
				else
					cutEnding(stem, RV, R2, endings[0], verb2);
			}
			else if (regex_match(RV, nounM))
				cutEnding(stem, RV, R2, endings[0], noun);
			if (regex_match(RV, iM))
				cutEnding(stem, RV, R2, endings[1], i);
			if (regex_match(R2, derivationalM))
				cutEnding(stem, RV, R2, endings[2], derivational);
			if (regex_match(RV, nnM))
				cutEnding(stem, RV, R2, endings[3], nn);
			else if (regex_match(RV, superlativeM))
			{
				cutEnding(stem, RV, R2, endings[3], superlative);
				if (regex_match(RV, nnM))
					cutEnding(stem, RV, R2, endings[3], nn);
			}
			else if (regex_match(RV, pM))
				cutEnding(stem, RV, R2, endings[3], p);
		}
		stem_ = stem;
		for (int i = 0; i < 4; i++)
		{
			endings_[i] = endings[i];
		}
		return;
	}

	void LemmatizationImpl::cutEnding(string &stem, string &RV, string &R2, string &ending, const regex rgx)
	{
		string tempRV = RV;
		RV = regex_replace(RV, rgx, "");
		R2 = regex_replace(R2, rgx, "");
		ending = tempRV.substr(RV.length());
		stem = stem.substr(0, stem.length() - ending.length());
	}

	string LemmatizationImpl::findRV(string inputWord)
	{
		string RV;
		size_t firstVowelPosInWord = inputWord.find_first_of(vowels, 0);

		if (firstVowelPosInWord != string::npos)
		{
			RV = inputWord.substr(firstVowelPosInWord + 1);
		}
		return RV;
	}

	string LemmatizationImpl::findR1(string RV)
	{
		string R1 = RV;
		size_t firstVowelPosInR1;
		if (!RV.empty())
		{
			firstVowelPosInR1 = R1.find_first_of(vowels, 0);
			if (firstVowelPosInR1 != string::npos && firstVowelPosInR1 != 0)
			{
				R1 = R1.substr(1);
				return R1;
			}
		}
		return findR2(R1);
	}

	string LemmatizationImpl::findR2(string R1)
	{
		string R2 = R1;
		size_t firstVowelPosInR1;
		size_t firstConsonantsPosInR1;
		firstVowelPosInR1 = R2.find_first_of(vowels, 0);
		firstConsonantsPosInR1 = R2.find_first_of(consonants, 0);
		while (firstVowelPosInR1 != string::npos && firstConsonantsPosInR1 != string::npos && !R2.empty() && (firstConsonantsPosInR1 != firstVowelPosInR1 + 1))
		{
			if (firstConsonantsPosInR1 < firstVowelPosInR1)
			{
				R2 = R2.substr(firstConsonantsPosInR1 + 1);
			}
			else
			{
				R2 = R2.substr(firstVowelPosInR1 + 1);
			}
			firstVowelPosInR1 = R2.find_first_of(vowels, 0);
			firstConsonantsPosInR1 = R2.find_first_of(consonants, 0);
		}
		if (firstVowelPosInR1 == string::npos || firstConsonantsPosInR1 == string::npos || R2.empty())
		{
			return string();
		}
		else
		{
			return R2.substr(firstConsonantsPosInR1 + 1);
		}
		return R2;
	}
}
