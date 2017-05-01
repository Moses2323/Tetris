#include "tableForTetris.h"

// ------------------------- TetrisElement

ostream& operator<<(ostream& s, const TetrisElement& elem){
	s<<"[te: maxW="<<elem.maxWidth_<<", mE=("<<elem.mainElement_.first<<","<<elem.mainElement_.second<<")";
	s<<", DHtR="<<elem.defaultShiftingToRight_;
	size_t N = elem.elements_.size();

	for(size_t i=0; i<N; ++i){
		s<<" (";
		s<<elem.elements_[i].first<<", "<<elem.elements_[i].second<<")";
	}

	s<<"]";
	return s;
}

void TetrisElement::addSquare(int row, int column){
	if (column < 0) { print_mistaka(toStr(__FUNCTION__)+"(int,int)", __LINE__, __FILE__, "Sorry, but I can\'t create element with column < 0. Your column number = " + toStr(column) + "\n\tthis element "+toStr(*this)); }
	if (column >= maxWidth_) { print_mistaka(toStr(__FUNCTION__)+"(int,int)", __LINE__, __FILE__, "Sorry, but I can\'t create element with column >= maxWidth . Your column number = " + toStr(column) + "\n\tthis element "+toStr(*this)); }
	if (row < 0) { print_mistaka(toStr(__FUNCTION__)+"(int,int)", __LINE__, __FILE__, "Sorry, but I can\'t create element with row < 0. Your row number = " + toStr(row) + "\n\tthis element "+toStr(*this)); }

	elements_.push_back(pair<int,int>(row,column));

	if (elements_.size() == 1)
		mainElement_ = elements_[0];
}

TetrisElement::TetrisElement(const TetrisElement &other)
: maxWidth_(other.maxWidth_), defaultShiftingToRight_(other.defaultShiftingToRight_), defaultShiftingToDown_(other.defaultShiftingToDown_), canTwist_(other.canTwist_), mainElement_(other.mainElement_), elements_(other.elements_.size()){

	size_t N = other.elements_.size();
	for(size_t i=0; i<N; ++i){
		elements_[i] = other.elements_[i];
	}

}

TetrisElement& TetrisElement::operator =(const TetrisElement& rhs){
	maxWidth_ = rhs.maxWidth_;
	defaultShiftingToRight_ = rhs.defaultShiftingToRight_;
	defaultShiftingToDown_ = rhs.defaultShiftingToDown_;
	canTwist_ = rhs.canTwist_;
	mainElement_ = rhs.mainElement_;

	size_t N = rhs.elements_.size();
	elements_.resize(N);
	for(size_t i=0; i<N; ++i){
		elements_[i] = rhs.elements_[i];
	}
	return *this;
}

int TetrisElement::findMinColumn() const{
	int minW = numeric_limits<int>::max();
	for(size_t i=0; i<elements_.size(); ++i){
		if (elements_[i].second < minW){
			minW = elements_[i].second;
		}
	}
	return minW;
}
int TetrisElement::findMaxColumn() const{
	int maxW = -numeric_limits<int>::max();
	for(size_t i=0; i<elements_.size(); ++i){
		if (elements_[i].second > maxW){
			maxW = elements_[i].second;
		}
	}
	return maxW;
}
int TetrisElement::findMinRow() const{
	int minW = numeric_limits<int>::max();
	for(size_t i=0; i<elements_.size(); ++i){
		if (elements_[i].first < minW){
			minW = elements_[i].first;
		}
	}
	return minW;
}

void TetrisElement::setMainElement(int Mrow, int Mcol) {
	bool checked = false;
	for(size_t i=0; i<elements_.size(); ++i){
		if ((Mrow == elements_[i].first) && (Mcol == elements_[i].second)){
			checked = true;
			break;
		}
	}
	if (!checked){
		print_mistaka(toStr(__FUNCTION__)+"(int,int)", __LINE__, __FILE__,
		 "Main element of tetris element must have coordinates from one of cubeElement in TetrisElement.element_ .\n\tYour element = ("+toStr(Mrow)+","+toStr(Mcol)+").\n\tthis Tetris Element "+toStr(*this));
	}
	mainElement_.first = Mrow;
	mainElement_.second = Mcol;
}

void TetrisElement::toLeft(){
	int minW = findMinColumn();
	if (minW == 0) {return;}

	for(size_t i=0; i<elements_.size(); ++i){
		--(elements_[i].second);
	}
	--(mainElement_.second);
}

void TetrisElement::toRight(){
	int maxW = findMaxColumn();
	if (maxW == maxWidth_ - 1) {return;}			// -1 nado li ?

	for(size_t i=0; i<elements_.size(); ++i){
		++(elements_[i].second);
	}
	++(mainElement_.second);
}

void TetrisElement::toDown(){
	for(size_t i=0; i<elements_.size(); ++i){
		++(elements_[i].first);
	}
	++(mainElement_.first);
}

bool TetrisElement::checkAllElements() const{
	for(size_t i=0; i<elements_.size(); ++i){
		if (elements_[i].first < 0)
			return false;
		if (elements_[i].second < 0)
			return false;
		if (elements_[i].second >= maxWidth_)
			return false;
	}
	return true;
}

void TetrisElement::clockwise(){
	if (!canTwist_) {return;}

	int r, c;
	for(size_t i=0; i<elements_.size(); ++i){
		r = elements_[i].first;
		c = elements_[i].second;
		elements_[i].first = c - mainElement_.second + mainElement_.first;
		elements_[i].second = -r + mainElement_.first + mainElement_.second;
	}
	if (!checkAllElements()){
		for(size_t i=0; i<elements_.size(); ++i){		//counterclockwise
			r = elements_[i].first;
			c = elements_[i].second;
			elements_[i].first = -c + mainElement_.second + mainElement_.first;
			elements_[i].second = r - mainElement_.first + mainElement_.second;
		}
	}
}

void TetrisElement::counterclockwise(){
	if (!canTwist_) {return;}

	int r, c;
	for(size_t i=0; i<elements_.size(); ++i){
		r = elements_[i].first;
		c = elements_[i].second;
		elements_[i].first = -c + mainElement_.second + mainElement_.first;
		elements_[i].second = r - mainElement_.first + mainElement_.second;
	}
	if (!checkAllElements())
		clockwise();
}

void TetrisElement::doDefaultShifting(){
	for(int i=0; i<defaultShiftingToRight_; ++i){
		toRight();
	}
	for(int i=0; i<defaultShiftingToDown_; ++i){
		toDown();
	}
}

int TetrisElement::findMaxRow() const{
	int maxRow = -1;
	for(int i=0; i<elements_.size(); ++i){
		if (elements_[i].first > maxRow){
			maxRow = elements_[i].first;
		}
	}
	return maxRow;
}

// ------------------------- DitschTable

void TableForTetris::setSpeed(int level){
	if (level <= 0)
		maxCounterDown_ = 1;
	else
		maxCounterDown_ = level;
}

void TableForTetris::initDefaultElements(){
	if (Ncol_ < 4){
		print_mistaka(toStr(__FUNCTION__)+"()", __LINE__, __FILE__,
		   "Can\'t create standart figures with number of columns less than 4. Your Ncol = "+toStr(Ncol_));
	}
	if (Nrow_ < 4){
		print_mistaka(toStr(__FUNCTION__)+"()", __LINE__, __FILE__,
		   "Can\'t create standart figures with number of rows less than 4. Your Nrow = "+toStr(Nrow_));
	}
	standartElements_.reserve(7);

	int shift = Ncol_/2 - 2;
	cout<<"default shift = "<<shift<<endl;

	TetrisElement te;								// 0000
	te.addSquare(0,0);
	te.addSquare(0,1);
	te.addSquare(0,2);
	te.addSquare(0,3);
	te.setMainElement(0,1);
	te.setDefaultShiftingToDown(1);
	te.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te);

	TetrisElement te2;								// 0
	te2.addSquare(0,0);								// 000
	te2.addSquare(1,0);
	te2.addSquare(1,1);
	te2.addSquare(1,2);
	te2.setMainElement(1,1);
	te2.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te2);

	TetrisElement te3;								//   0
	te3.addSquare(1,0);								// 000
	te3.addSquare(1,1);
	te3.addSquare(1,2);
	te3.addSquare(0,2);
	te3.setMainElement(1,1);
	te3.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te3);

	TetrisElement te4;								// 00
	te4.addSquare(0,0);								// 00
	te4.addSquare(0,1);
	te4.addSquare(1,0);
	te4.addSquare(1,1);
	te4.setTwist(false);
	te4.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te4);

	TetrisElement te5;								//  00
	te5.addSquare(1,0);								// 00
	te5.addSquare(1,1);
	te5.addSquare(0,1);
	te5.addSquare(0,2);
	te5.setMainElement(0,1);
	te5.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te5);

	TetrisElement te6;								//  0
	te6.addSquare(1,0);								// 000
	te6.addSquare(1,1);
	te6.addSquare(0,1);
	te6.addSquare(1,2);
	te6.setMainElement(1,1);
	te6.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te6);

	TetrisElement te7;								// 00
	te7.addSquare(0,0);								//  00
	te7.addSquare(0,1);
	te7.addSquare(1,1);
	te7.addSquare(1,2);
	te7.setMainElement(0,1);
	te7.setDefaultShiftingToRight(shift);
	standartElements_.push_back(te7);

	for(size_t i=0; i<standartElements_.size(); ++i){
		standartElements_[i].setMaxWidth(Ncol_);
		standartElements_[i].doDefaultShifting();
		cout<<"standartElement["<<i<<"] = "<<standartElements_[i]<<endl;
	}
	cout<<endl;
	distr_ = uniform_int_distribution<int>(0, static_cast<int>(standartElements_.size()-1));
}

void TableForTetris::deleteCurrentElement(){
	for(int i=0; i<Nrow_; ++i){
		for(int k=0; k<Ncol_; ++k){
			if (table_->item(i,k)->text() == movingElement_.second){
				table_->item(i,k)->setBackground(usualElement_.first);
				table_->item(i,k)->setTextColor(usualElement_.first);
				table_->item(i,k)->setText(usualElement_.second);
			}
		}
	}
}

void TableForTetris::drawCurrentElement(){
	size_t N = currentElement_.getNumberOfSquares();
	int row, col;
	for(size_t i=0; i<N; ++i){
		row = currentElement_.getElem(i).first;
		col = currentElement_.getElem(i).second;
		table_->item(row, col)->setBackground(movingElement_.first);
		table_->item(row, col)->setTextColor(movingElement_.first);
		table_->item(row, col)->setText(movingElement_.second);
	}
}

void TableForTetris::allRegulatingButtonsSetDisable(bool flag){
	leftArrow_->setDisabled(flag);
	rightArrow_->setDisabled(flag);
	speedUp_->setDisabled(flag);
	clockwiseButton_->setDisabled(flag);
}

void TableForTetris::startIt(){
	allRegulatingButtonsSetDisable(false);
	paused_ = false;

	timerUpdateTable_->setInterval(msPeriod_);
	timerUpdateTable_->start();
	okButton_->hide();
	cancelButton_->show();
	cout<<"start ends"<<endl;
	setFocus();
}

void TableForTetris::cancelIt(){
	paused_ = true;
	timerUpdateTable_->stop();
	cancelButton_->hide();
	okButton_->show();

	allRegulatingButtonsSetDisable(true);
}

void TableForTetris::currentToStatic(){
	cout<<"currentToStatic..."<<endl;
	for(int i=0; i<Nrow_; ++i){
		for(int k=0; k<Ncol_; ++k){
			if (table_->item(i,k)->text() == movingElement_.second){
				table_->item(i,k)->setBackground(staticElement_.first);
				table_->item(i,k)->setTextColor(staticElement_.first);
				table_->item(i,k)->setText(staticElement_.second);
			}
		}
	}
	cout<<"currentToStatic"<<endl;
}

bool TableForTetris::checkIfNeedToStatic() const{
	size_t N = currentElement_.getNumberOfSquares();
	int row, col;
	for(size_t i=0; i<N; ++i){
		row = currentElement_.getElem(i).first;
		col = currentElement_.getElem(i).second;
		if ((row == Nrow_-1) ||
		    (table_->item(row+1,col)->text() == staticElement_.second)){
			return true;
		}
	}
	return false;
}

void TableForTetris::cleanAfterMig(){
	int Ndesolving = static_cast<int>(mustDesolve_.size());
	for(int i=Ndesolving-1; i>=0; --i){
		for(int k=i-1; k>=0; --k){
			++mustDesolve_[k];
		}
		for(int m=mustDesolve_[i]; m > 0; --m){
			for(int col = 0; col<Ncol_; ++col){
				table_->item(m, col)->setBackground(table_->item(m-1,col)->backgroundColor());		//!!!!
				table_->item(m,col)->setTextColor(table_->item(m-1, col)->backgroundColor());		//!!!!
				table_->item(m, col)->setText(table_->item(m-1,col)->text());
				table_->item(m-1,col)->setBackground(usualElement_.first);
				table_->item(m-1,col)->setTextColor(usualElement_.first);
				table_->item(m-1,col)->setText(usualElement_.second);
			}
		}
	}
	emit plusScore(5*Ndesolving*(Ndesolving+1));
}

void TableForTetris::miganie(){
	++counterMig_;
	int row;
	if (counterMig_ % 2 == 1){
		for(size_t k=0; k<mustDesolve_.size(); ++k){
			row = mustDesolve_[k];
			for(int m=0; m<Ncol_; ++m){
				table_->item(row,m)->setBackground(deletingElement_.first);
				table_->item(row,m)->setTextColor(deletingElement_.first);
				table_->item(row,m)->setText(deletingElement_.second);
			}
		}
	}
	else{
		for(size_t k=0; k<mustDesolve_.size(); ++k){
			row = mustDesolve_[k];
			for(int m=0; m<Ncol_; ++m){
				table_->item(row,m)->setBackground(staticElement_.first);
				table_->item(row,m)->setTextColor(staticElement_.first);
				table_->item(row, m)->setText(staticElement_.second);
			}
		}
	}

	if (counterMig_ >= NtimesMig_){
		counterMig_ = 0;
		for(size_t k=0; k<mustDesolve_.size(); ++k){
			row = mustDesolve_[k];
			for(int m=0; m<Ncol_; ++m){
				table_->item(row,m)->setBackground(usualElement_.first);
				table_->item(row,m)->setTextColor(usualElement_.first);
				table_->item(row, m)->setText(usualElement_.second);
			}
		}
		timerMig_->stop();

		cleanAfterMig();

		timerUpdateTable_->start();
	}
}

void TableForTetris::desolveIt(){
	cout<<"desolving..."<<endl;
	timerUpdateTable_->stop();
	timerMig_->start();
	cout<<"desolved"<<endl;
}

void TableForTetris::checkLines(){
	cout<<"checking lines..."<<endl;
	mustDesolve_.resize(0);

	bool flagMustDesolve;
	for(int i=0; i<Nrow_; ++i){
		flagMustDesolve = true;
		for(int k=0; k<Ncol_; ++k){
			if (table_->item(i,k)->text() != staticElement_.second){
				flagMustDesolve = false;
				break;
			}
		}
		if (flagMustDesolve){
			mustDesolve_.push_back(i);
		}
	}
	if (mustDesolve_.size() > 0)
		desolveIt();
	cout<<"checking lines"<<endl;
}

void TableForTetris::tryToDown(){
	if (!doNotTouchCurrentElement_){
		doNotTouchCurrentElement_ = true;
		if (checkIfNeedToStatic()){
			currentToStatic();
			doNotTouchCurrentElement_ = false;

			checkLines();

			currentElement_ = nextElement_;
			nextElement_ = standartElements_[choser()];
			cout<<endl<<"NEXT ELEMENT"<<endl;
			emit anotherNextElement(nextElement_);
			newElement_ = true;
		}
		else{
			currentElement_.toDown();
		}
		doNotTouchCurrentElement_ = false;
	}
}

void TableForTetris::clearAll(){
	for(int i=0; i<Nrow_; ++i){
		for(int k=0; k<Ncol_; ++k){
			table_->item(i,k)->setBackground(usualElement_.first);
			table_->item(i,k)->setText(usualElement_.second);
		}
	}
}

void TableForTetris::checkLoosing(){
	for(int i=0; i<Ncol_; ++i){
		if (table_->item(3,i)->text() == staticElement_.second){
			emit gameOver();
			clearAll();
			cancelIt();
		}
	}
}

void TableForTetris::changeTable(){
	++counterDown_;
	if (counterDown_ >= maxCounterDown_){
		counterDown_ = 0;
		if (!newElement_)
			tryToDown();
		checkLoosing();
	}
	deleteCurrentElement();
	if (!newElement_){
		drawCurrentElement();
	}
	newElement_ = false;
}

bool TableForTetris::collision() const{
	size_t N = currentElement_.getNumberOfSquares();
	int row, col;
	for(size_t i=0; i<N; ++i){
		row = currentElement_.getElem(i).first;
		col = currentElement_.getElem(i).second;
		if (table_->item(row,col)->text() == staticElement_.second){
			return true;
		}
	}
	return false;
}

void TableForTetris::toLeft(){
	if (!doNotTouchCurrentElement_){
		doNotTouchCurrentElement_ = true;
		cout<<"to Left... ";
		currentElement_.toLeft();
		if (collision()){
			currentElement_.toRight();
		}
		changeTable();
		cout<<"done"<<endl;
		doNotTouchCurrentElement_ = false;
	}
}

void TableForTetris::toRight(){
	if (!doNotTouchCurrentElement_){
		doNotTouchCurrentElement_ = true;
		cout<<"to Right...";
		currentElement_.toRight();
		if (collision()){
			currentElement_.toLeft();
		}
		changeTable();
		cout<<"done"<<endl;
		doNotTouchCurrentElement_ = false;
	}
}

void TableForTetris::clockwise(){
	if (!doNotTouchCurrentElement_){
		doNotTouchCurrentElement_ = true;
		cout<<"clockwise...";
		currentElement_.clockwise();
		if (collision()){
			currentElement_.counterclockwise();
		}
		changeTable();
		cout<<"done"<<endl;
		doNotTouchCurrentElement_ = false;
	}
}

void TableForTetris::speedUp(){
	rememberedToSpeedDown_ = maxCounterDown_;
	maxCounterDown_ = 1;
}

void TableForTetris::speedToNormal(){
	maxCounterDown_ = rememberedToSpeedDown_;
}

void TableForTetris::keyPressEvent(QKeyEvent *ev){
	cout<<"Event!"<<endl;
	if (!paused_){
		switch( ev->key() ){
			case Qt::Key_A:
				cout<<"\tKey A pressed"<<endl;
				toLeft();
				break;
			case Qt::Key_D:
				cout<<"\tKey D pressed"<<endl;
				toRight();
				break;
			case Qt::Key_W:
				cout<<"\tKey W pressed"<<endl;
				clockwise();
				break;
			case Qt::Key_Space:
				cout<<"\tKey Space pressed"<<endl;
				speedUp();
				break;
			case Qt::Key_P:
				cout<<"\tKey P pressed"<<endl;
				if (okButton_->isVisible())
					startIt();
				else
					cancelIt();
				break;

		//	default:
		//		QWidget::keyPressEvent(ev);
		}
		ev->accept();
	}
	else{
		if (ev->key() == Qt::Key_P){
			cout<<"\tKey P pressed"<<endl;
			if (okButton_->isVisible())
				startIt();
			else
				cancelIt();
		}
	}
}

void TableForTetris::keyReleaseEvent(QKeyEvent *ev){
	if (!paused_){
		switch( ev->key() ){
			case Qt::Key_Space:
				cout<<"Key Space was released"<<endl;
				speedToNormal();
				break;
		//	default:
		//		QWidget::keyReleaseEvent(ev);
		}
		ev->accept();
	}
}

TableForTetris::TableForTetris(int Nrows, int Ncols, QWidget *pwg) : QWidget(pwg), Nrow_(Nrows), Ncol_(Ncols){
	initDefaultElements();
	nextElement_ = standartElements_[choser()];
	currentElement_ = standartElements_[choser()];

	mL_ = new QVBoxLayout;
	arrowLayout_ = new QHBoxLayout;
	table_ = new QTableWidget(Nrow_,Ncol_);

	table_->setEditTriggers(QTableWidget::NoEditTriggers);
	table_->setEnabled(false);

	okButton_ = new QPushButton(tr("Ok"));
	cancelButton_ = new QPushButton(tr("Cancel"));
	leftArrow_ = new QPushButton(tr("Left Arrow"));
	clockwiseButton_ = new QPushButton(tr("Clockwise"));
	rightArrow_ = new QPushButton(tr("Right Arrow"));
	speedUp_ = new QPushButton(tr("Speed Up"));

	cancelButton_->hide();
	timerUpdateTable_ = new QTimer(this);
	timerMig_ = new QTimer(this);
	timerMig_->setInterval(msDurMig_);

	staticElement_.first = QColor(1,1,1);
	staticElement_.second = "1";
	movingElement_.first = QColor(1,1,1);
	movingElement_.second = "2";
	deletingElement_.first = QColor(Qt::red);
	deletingElement_.second = "3";
	usualElement_.first = QColor(Qt::white);

	for(int i=0; i<Ncol_; ++i){
		table_->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
		table_->horizontalHeader()->resizeSection(i,40);
	}

	for(int i=0; i<Nrow_; ++i){
		for(int k=0; k<Ncol_; ++k){
			QTableWidgetItem* twi = new QTableWidgetItem;
			twi->setBackground(usualElement_.first);
			twi->setText(usualElement_.second);
			table_->setItem(i,k,twi);
		}
	}
	table_->hideRow(0);
	table_->hideRow(1);
	table_->hideRow(2);
	table_->hideRow(3);

	table_->horizontalHeader()->hide();
	table_->verticalHeader()->hide();

	arrowLayout_->addWidget(leftArrow_);
	arrowLayout_->addWidget(clockwiseButton_);
	arrowLayout_->addWidget(rightArrow_);

	mL_->addWidget(speedUp_);
	mL_->addLayout(arrowLayout_);
	mL_->addWidget(table_);
	mL_->addWidget(okButton_);
	mL_->addWidget(cancelButton_);
	setLayout(mL_);

	setGeometry(50,50,1000,1000);

	connect(leftArrow_, SIGNAL(clicked()), this, SLOT(toLeft()));
	connect(clockwiseButton_, SIGNAL(clicked()), this, SLOT(clockwise()));
	connect(rightArrow_, SIGNAL(clicked()), this, SLOT(toRight()));
	connect(okButton_, SIGNAL(clicked()), this, SLOT(startIt()));
	connect(cancelButton_, SIGNAL(clicked()), this, SLOT(cancelIt()));
	connect(timerUpdateTable_, SIGNAL(timeout()), this, SLOT(changeTable()));
	connect(timerMig_, SIGNAL(timeout()), this, SLOT(miganie()));
	connect(speedUp_, SIGNAL(pressed()), this, SLOT(speedUp()));
	connect(speedUp_, SIGNAL(released()), this, SLOT(speedToNormal()));

	allRegulatingButtonsSetDisable(true);
}
