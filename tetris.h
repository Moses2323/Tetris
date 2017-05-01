#ifndef TETRIS_GASPARYANMOSES_09042017
#define TETRIS_GASPARYANMOSES_09042017

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include "tableForTetris.h"

class StrangeTetris : public QWidget{
	Q_OBJECT
private:
	QHBoxLayout* mL_;
	QVBoxLayout* auxLayout_;
	TableForTetris* tableForTetris_;
	QTableWidget* nextElem_;
	QLabel* scoreLab_;
	QLineEdit* score_;
	QLabel* speedLab_;
	QSlider* speedLevel_;

	int scoreInt_{0};
	int NnextElemRows_{5};
	int NnextElemCols_{5};
	int maxSpeedLevel_{20};

	void clearField();

public:
	StrangeTetris(int Nrows = 24, int Ncols = 10, QWidget* pwg = Q_NULLPTR);
	~StrangeTetris() {}

protected:
	virtual void keyPressEvent(QKeyEvent *ev){
		tableForTetris_->keyPressEvent(ev);
	}
	virtual void keyReleaseEvent(QKeyEvent *ev){
		tableForTetris_->keyReleaseEvent(ev);
	}

public slots:
	void plusScore(int score);
	void changePic(TetrisElement te);
	void gameOver();
};

#endif // TETRIS_GASPARYANMOSES_09042017
