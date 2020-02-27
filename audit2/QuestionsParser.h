#pragma once

#include "tinyxml2.h"
#include "Question.h"

#include <string>

/*#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XMLError::XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif
*/
#ifndef XMLCheckPointer
#define XMLCheckPointer(ptr) if (ptr == nullptr) {throw XMLError::XML_ERROR_PARSING_ELEMENT;}
#endif

class QuestionsParser
{
public:
	enum class Node { ROOT, EASY, MEDIUM, HARD, VERYHARD };

	QuestionsParser();
	QuestionsParser(std::string path);
	~QuestionsParser();

	void setPath(std::string path);
	std::string getPath();

	void load(); /* throws XMLError */
	void load(std::string path); /* throws XMLError */

	void read(Question& q); /* throws XMLError */

	/** Read a specific question specified by its index.
	  * If the index is out of bounds (higher than {@link QuestionsParser::count()} -1), this reads the next question.
	  * @throws XMLError if the xml is malformed
	  **/
	void read(Question& q, unsigned index);

	unsigned count(Question::Level diff);/* throws XMLError */
	unsigned countTotal();/* throws XMLError */
	unsigned getOffset(Question::Level diff);/* throws XMLError */

private:
	void nextQuestion(); /* throws XMLError */
	void prevQuestion(); /* throws XMLError */
	void nextDifficulty(); /* throws XMLError */
	void prevDifficulty(); /* throws XMLError */

	std::string _path;
	tinyxml2::XMLDocument _doc;
	tinyxml2::XMLNode* _root;
	tinyxml2::XMLElement* _currentChild;
	tinyxml2::XMLNode* _currentDifficulty;

};

