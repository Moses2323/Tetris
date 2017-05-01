#ifndef WIDGET_GASPARYANMOSES_08042017
#define WIDGET_GASPARYANMOSES_08042017

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QKeyEvent>

#include <vector>
#include <limits>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
using namespace std;

template<typename T>
inline string toStr(const T& elem){
	stringstream vss;
	vss<<elem;
	return vss.str();
}

inline void print_mistaka(const string& funcName, int line, const string& filename, const string& message){
	cerr<<"ERROR in "<<funcName<<" : line "<<line<<", file "<<filename<<endl;
	cerr<<"\t"<<message<<endl;
	throw string("mistaka");
}
inline void print_mistaka_lite(const string& funcName, int line, const string& filename, const string& message){
	cerr<<"WARNING in "<<funcName<<" : line "<<line<<", file "<<filename<<endl;
	cerr<<"\t"<<message<<endl;
}

class TetrisElement final{

private:
	int maxWidth_ {10};
	int defaultShiftingToRight_ {0};
	int defaultShiftingToDown_ {0};
	bool canTwist_ {true};
	pair<int,int> mainElement_;
	vector<pair<int,int> > elements_;		// <row, column>

	bool checkAllElements() const;

public:
	TetrisElement() : mainElement_(0,0) { elements_.reserve(5);}
	TetrisElement(const TetrisElement& other);
	~TetrisElement() {}

	TetrisElement& operator=(const TetrisElement& rhs);

	void addSquare(int row, int column);

	void setMaxWidth(int w) { maxWidth_ = w; }
	const int& getMaxWidth() const { return maxWidth_; }
	void setMainElement(int Mrow, int Mcol);
	const pair<int,int>& getMainElement() const { return mainElement_; }
	void setDefaultShiftingToRight(int sh) { defaultShiftingToRight_ = sh; }
	const int& getDefaultShiftingToRight() const { return defaultShiftingToRight_; }
	void setDefaultShiftingToDown(int sh) { defaultShiftingToDown_ = sh; }
	const int& getDefaultShiftingToDown() const { return defaultShiftingToDown_; }
	void setTwist(bool tw) { canTwist_ = tw; }
	const bool& getTwist() const { return canTwist_; }

	size_t getNumberOfSquares() const { return elements_.size(); }
	const pair<int,int>& getElem(const size_t& ind) const { return elements_[ind]; }

	void toLeft();
	void toRight();
	void toDown();
	void clockwise();
	void counterclockwise();
	void doDefaultShifting();

	int findMaxRow() const;
	int findMinRow() const;
	int findMinColumn() const;
	int findMaxColumn() const;

	friend ostream& operator<<(ostream& s, const TetrisElement& elem);
};
ostream& operator<<(ostream& s, const TetrisElement& elem);

class TableForTetris : public QWidget{
	Q_OBJECT
private:
	QTableWidget* table_;
	QVBoxLayout* mL_;
	QTimer* timerUpdateTable_;

	int msPeriod_ {50};
	int maxCounterDown_{10};	//10
	int counterDown_{0};
	int rememberedToSpeedDown_;

	bool paused_{true};

	QPushButton* speedUp_;
	QPushButton* okButton_;
	QPushButton* cancelButton_;

	QPushButton* leftArrow_;
	QPushButton* clockwiseButton_;
	QPushButton* rightArrow_;
	QHBoxLayout* arrowLayout_;

	pair<QColor, QString> staticElement_;
	pair<QColor, QString> movingElement_;
	pair<QColor, QString> usualElement_;
	pair<QColor, QString> deletingElement_;

	vector<TetrisElement> standartElements_;
	mt19937 randomer_ {random_device()()};
	uniform_int_distribution<int> distr_;

	int NtimesMig_{10};	//number of blinks
	int msDurMig_{100};	//period in milliseconds for blinks
	int counterMig_{0};	//don't touch this
	vector<int> mustDesolve_;
	QTimer* timerMig_;
	bool newElement_{false};

	int Nrow_ {24};		//must be more than 6!
	int Ncol_ {10};		//must be >=4

	bool doNotTouchCurrentElement_{false};

	TetrisElement currentElement_;
	TetrisElement nextElement_;

	void initDefaultElements();
	int choser() { return distr_(randomer_); }

	bool collision() const;
	void deleteCurrentElement();
	void currentToStatic();
	void drawCurrentElement();

	bool checkIfNeedToStatic() const;
	void desolveIt();
	void checkLines();
	void cleanAfterMig();

	void clearAll();
	void checkLoosing();
	void allRegulatingButtonsSetDisable(bool flag);

public:
	virtual void keyPressEvent(QKeyEvent* ev) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;

public:
	TableForTetris(int Nrows, int Ncols, QWidget* pwg = Q_NULLPTR);
	~TableForTetris() {}

	const TetrisElement& getNextElement() const { return nextElement_; }


public slots:
	void toLeft();
	void toRight();
	void clockwise();
	void startIt();
	void cancelIt();

	void changeTable();
	void tryToDown();
	void miganie();

	void speedUp();
	void speedToNormal();

	void setSpeed(int level);

signals:
	void gameOver();
	void plusScore(int);
	void anotherNextElement(TetrisElement);
};

#endif // WIDGET_GASPARYANMOSES_08042017
