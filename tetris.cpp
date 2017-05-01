#include "tetris.h"

void StrangeTetris::plusScore(int score){
	scoreInt_ += score;
	stringstream vss;
	vss<<scoreInt_;
	score_->setText(QString::fromStdString(vss.str()));
}


void StrangeTetris::clearField(){
	for(int i=0; i<NnextElemRows_; ++i){
		for(int k=0; k<NnextElemCols_; ++k){
			nextElem_->item(i,k)->setBackground(QColor(Qt::white));
			nextElem_->item(i,k)->setText("");
		}
	}
}

void StrangeTetris::changePic(TetrisElement te){
	int minR = te.findMinRow();
	int minC = te.findMinColumn();
	clearField();

	size_t N = te.getNumberOfSquares();
	for(size_t i=0; i<N; ++i){
		nextElem_->item(te.getElem(i).first - minR + 1, te.getElem(i).second - minC + 1)->setBackground(QColor(Qt::black));
	}
}

void StrangeTetris::gameOver(){
	QMessageBox::information(this, tr(""), tr("Game Over!\nYour score: ")+score_->text());
	scoreInt_ = 0;
	score_->setText("0");
}

StrangeTetris::StrangeTetris(int Nrows, int Ncols, QWidget *pwg) : QWidget(pwg){
	mL_ = new QHBoxLayout;
	auxLayout_ = new QVBoxLayout;
	tableForTetris_ = new TableForTetris(Nrows, Ncols);
	speedLevel_ = new QSlider;
	speedLab_ = new QLabel(tr("Speed Level:"));
	speedLevel_->setOrientation(Qt::Horizontal);

	nextElem_ = new QTableWidget(NnextElemRows_,NnextElemCols_);
	nextElem_->setEditTriggers(QTableWidget::NoEditTriggers);
	nextElem_->horizontalHeader()->hide();
	nextElem_->verticalHeader()->hide();

	scoreLab_ = new QLabel(tr("Score"));
	QFont fnt;
	fnt.setPointSize(80);
	scoreLab_->setFont(fnt);
	score_ = new QLineEdit;
	score_->setFont(fnt);
	fnt.setPointSize(20);
	speedLab_->setFont(fnt);

	score_->setReadOnly(true);
	score_->setText("0");

	speedLevel_->setMinimum(1);
	speedLevel_->setMaximum(maxSpeedLevel_);

	auxLayout_->addWidget(nextElem_);
	auxLayout_->addWidget(speedLab_);
	auxLayout_->addWidget(speedLevel_);
	auxLayout_->addWidget(scoreLab_);
	auxLayout_->addWidget(score_);

	mL_->addWidget(tableForTetris_, 10);
	mL_->addLayout(auxLayout_, 1);
	setLayout(mL_);

	for(int i=0; i<NnextElemRows_; ++i){
		for(int k=0; k<NnextElemCols_; ++k){
			QTableWidgetItem* twi = new QTableWidgetItem;
			twi->setBackground(QColor(Qt::white));
			twi->setText("");
			nextElem_->setItem(i,k,twi);
		}
	}

	for(int i=0; i<NnextElemCols_; ++i){
		nextElem_->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
		nextElem_->horizontalHeader()->resizeSection(i,40);
	}

	changePic(tableForTetris_->getNextElement());

	connect(tableForTetris_, SIGNAL(plusScore(int)), this, SLOT(plusScore(int)));
	connect(tableForTetris_, SIGNAL(anotherNextElement(TetrisElement)), this, SLOT(changePic(TetrisElement)));
	connect(tableForTetris_, SIGNAL(gameOver()), this, SLOT(gameOver()));
	connect(speedLevel_, SIGNAL(valueChanged(int)), tableForTetris_, SLOT(setSpeed(int)));

	speedLevel_->setValue(maxSpeedLevel_/2);

	setGeometry(50,50,1000,900);

	nextElem_->setEnabled(false);
}
