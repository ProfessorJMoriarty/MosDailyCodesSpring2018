//magma simulator
//shows different molecules crystallizing and dropping at different rates as tempatures drop
//and liquifying at different high tempatures


//melting points
//1000 Olivine Fe2SiO4
//800 plagioclase CaAlSi3O8
// 600 Quartz SiO2 and Feldspar KAlSi3O8
#include<iostream>

#include<vector>

#include<ctime>

#include <allegro5/allegro.h>

#include<allegro5\allegro_primitives.h>

using namespace std;

//CONSTANTS

const int SCREEN_W = 500;

const int SCREEN_H = 500;

const float FPS = 60;

//declarations

int RandomDir();

class atom {

private:

	double xPos;

	double yPos;

	double xVel;

	double yVel;

	char type;

public:

	void initAtom(char AtomType);

	void printInfo();

	void drawAtom();

	void moveAtom();

	int getX();

	int getY();
	void changeX(int num);
	void changeY(int num);
};

class crystal {

private:

	char type; //Olivine, Feldspar, Quartz

public:

	void initCrystal(char crystalType);

	vector <atom> Atoms; //holds the atoms in a crystal

	vector <atom>::iterator iter;

	void freeze();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////main
int main() {

	srand(time(NULL));

	ALLEGRO_DISPLAY *display = NULL;

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	ALLEGRO_TIMER *timer = NULL;

	bool redraw = true;
	int temp = 0;
	cout << "enter temp" << endl;
	cin >> temp;
	al_init();

	al_init_primitives_addon();

	timer = al_create_timer(1.0 / FPS);

	display = al_create_display(SCREEN_W, SCREEN_H);

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);



	//initalize game objects

	crystal quartz1;

	quartz1.initCrystal('q');

	crystal quartz2;

	quartz2.initCrystal('q');

	crystal quartz3;

	quartz3.initCrystal('q');

	//test game objects data

	//for (quartz1.iter = quartz1.Atoms.begin(); quartz1.iter != quartz1.Atoms.end(); quartz1.iter++)

	//quartz1.iter->printInfo();



	while (1)

	{

		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			//update atom movement
			if (temp > 600) {
				for (quartz1.iter = quartz1.Atoms.begin(); quartz1.iter != quartz1.Atoms.end(); quartz1.iter++)

					quartz1.iter->moveAtom();

				for (quartz2.iter = quartz2.Atoms.begin(); quartz2.iter != quartz2.Atoms.end(); quartz2.iter++)

					quartz2.iter->moveAtom();

				for (quartz3.iter = quartz3.Atoms.begin(); quartz3.iter != quartz3.Atoms.end(); quartz3.iter++)

					quartz3.iter->moveAtom();
			}
			else {
				quartz1.freeze();
				quartz2.freeze();
				quartz3.freeze();
			}
			redraw = true;

		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

			break;

		}

		if (redraw && al_is_event_queue_empty(event_queue)) {

			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			//draw atoms

			for (quartz1.iter = quartz1.Atoms.begin(); quartz1.iter != quartz1.Atoms.end(); quartz1.iter++)

				quartz1.iter->drawAtom();

			for (quartz2.iter = quartz2.Atoms.begin(); quartz2.iter != quartz2.Atoms.end(); quartz2.iter++)

				quartz2.iter->drawAtom();

			for (quartz3.iter = quartz3.Atoms.begin(); quartz3.iter != quartz3.Atoms.end(); quartz3.iter++)

				quartz3.iter->drawAtom();

			al_flip_display();

		}

	}//end game loop



	al_destroy_timer(timer);

	al_destroy_display(display);

	al_destroy_event_queue(event_queue);

	return 0;

}/////////////////////////////////////////////////////////////////////////////////////////////////////////////end main



 //class crystal functions

void crystal::initCrystal(char crystalType) {

	type = crystalType;

	if (type == 'q') {

		atom atom1;

		atom1.initAtom('s');

		atom atom2;

		atom2.initAtom('o');

		atom atom3;

		atom3.initAtom('o');

		Atoms.push_back(atom1);

		Atoms.push_back(atom2);

		Atoms.push_back(atom3);

	}

}

void crystal::freeze() {

	//calculate midpoint of all atoms in crystal:
	double XPoint = 0;
	double YPoint = 0;
	int count = 0;
	for (iter = Atoms.begin(); iter != Atoms.end(); iter++) {
		XPoint += iter->getX();
		YPoint += iter->getY();
		count++;
	}

	XPoint /= count;
	YPoint /= count;
	cout << "midpoint is " << XPoint << " , " << YPoint << endl;

	//move each point towards the midpoint
	for (iter = Atoms.begin(); iter != Atoms.end(); iter++) {
		if (XPoint <iter->getX())
			iter->changeX(-1);
		if (XPoint >iter->getX() )
			iter->changeX(1);
		if (YPoint < iter->getY() )
			iter->changeY(-1);
		if (YPoint >iter->getY() )
			iter->changeY(1);

	}
}

//class atom functions

void atom::initAtom(char AtomType) {

	xPos = rand() % SCREEN_W;

	yPos = rand() % SCREEN_H;

	xVel = RandomDir();

	yVel = RandomDir();

	type = AtomType;

}

void atom::printInfo() {

	cout << "xPos is " << xPos << endl;

	cout << "yPos is " << yPos << endl;

}

void atom::drawAtom() {

	if (type == 'o')

		al_draw_filled_circle(xPos, yPos, 2, al_map_rgb(100, 100, 200));

	else if (type == 's')

		al_draw_filled_circle(xPos, yPos, 4, al_map_rgb(200, 100, 200));

}

void atom::moveAtom() {

	xPos += xVel;

	yPos += yVel;

	if (xPos > SCREEN_W || xPos < 0) {

		xVel *= -1;

		yVel *= RandomDir();

	}

	if (yPos > SCREEN_H || yPos < 0) {

		yVel *= -1;

		xVel *= RandomDir();

	}

}

int atom::getX() {

	return xPos;

}

int atom::getY() {

	return yPos;

}

void atom::changeX(int num) {
	xPos += num;
}

void atom::changeY(int num) {
	yPos += num;
}




//main functions

int RandomDir() {

	int num = rand() % 2;

	if (num == 1)

		return 1;

	else

		return -1;

}