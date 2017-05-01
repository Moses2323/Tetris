//#include "ditschTable.h"
#include "tetris.h"
#include <QApplication>

#include <random>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StrangeTetris st(24,10);
	st.show();

	return a.exec();
}
