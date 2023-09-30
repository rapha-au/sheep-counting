#include <iostream>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <ratio>
#include <chrono>
#include <cstring>
#include <ncurses.h>

class Timer{
	private:
		int  sheepSeconds;
		std::chrono::steady_clock::time_point s;
		std::chrono::steady_clock::time_point initialTimepoint;
	public:
		Timer(int s){
			this->initialTimepoint = std::chrono::steady_clock::now();
			this->sheepSeconds = s;
		}

		bool checkHitTime(std::chrono::steady_clock::time_point currentTimepoint){
			 std::chrono::duration<int> timeSpan = std::chrono::duration_cast<std::chrono::duration<int>>(currentTimepoint - this->initialTimepoint);
			
			if (this->getSecond() >= 1.0){
				//reset second
				this->s = std::chrono::steady_clock::now();
			}

			 if(timeSpan.count() >= this->sheepSeconds){
			 	return true;
			 }

			return false;
			 
		}
	
	int getTime(){
	std::chrono::duration<int> timeSpan = std::chrono::duration_cast<std::chrono::duration<int>>(std::chrono::steady_clock::now() - this->initialTimepoint);

			 return timeSpan.count();
		}
	
	
	double getSecond(){
		std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - this->s);
		
		return timeSpan.count();
	}
		
		
};

enum SheepPosition{
	Left,
	Top,
	Right
};

class Sheep{
	private:
		std::string sheepLinesCurrent[4][1];
		
		std::string sheepLines[4][1] = {
			{"   ________  "},
			{"  /       o\\ "},
			{" /\\_____/U\\_|"},
			{"   v   v     "},
		};

		std::string sheepLines2[4][1] = {
			{"   ________  "},
			{" \\/       -\\ "},
			{"  \\_____/U\\_|"},
			{"   <    >    "},
		};
		
	public:
	Sheep(){
		
	}


	void draw(int x, int y, int t){
		if(t == 1){
			for(int i=0; i<4; i++){
				mvprintw(y+i,x,"%s",this->sheepLines[i-1][1].c_str());
			}
		}else if(t == 2){
			for(int i=0; i<4; i++){
				mvprintw(y+i,x,"%s",this->sheepLines2[i-1][1].c_str());
			}
		}
	}
	
};


class Farm{
	private:
		Sheep sheep;
		SheepPosition sheepPos;
		std::string fence[7][1] = {
			{"|X|-----____        "},
			{"|X|        -----|XX|"},
			{"|X|----____     |  |"},
			{"|X|        -----|XX|"},
			{"|X|----____     |  |"},
			{"|X|        -----|XX|"},
			{"                |  |"},
		};
	public:
		Farm(){
			sheepPos = SheepPosition::Left;
		}
	
		void update(Timer timer){
			
			if(timer.getSecond() <= .33){
				erase();
				sheepPos = SheepPosition::Left;
				
			}else if(timer.getSecond() <= .66 && timer.getSecond() > .33){
				erase();
				sheepPos = SheepPosition::Top;
				
			}else if(timer.getSecond() <= .99 && timer.getSecond() > .66){
				erase();
				sheepPos = SheepPosition::Right;
			}
		}
	
		void draw_ground(int y, int terminalWidth){
			for(int i=1; i<terminalWidth; i++){
				mvprintw(y,i,"%s","_");
			}
		}
	
		void draw_fence(int fencex, int fencey){
		//Draw Fence
			for(int i=0; i<7; i++){
				mvprintw(fencey+i, fencex,"%s", this->fence[i-1][1].c_str());
			}
		}
		
	
		void draw_all(int groundW){
			
			//Ground
			this->draw_ground(20, groundW);
			
			//Fence
			this->draw_fence(35,16);
			
			//Draw Sheep
				switch(sheepPos){
				case Left:
					sheep.draw(1,19, 1);
				break;
				
				case Top:
					sheep.draw(35,10, 2);
				break;
					
				case Right:
					sheep.draw(65,19, 1);
				break;
					
				default:
				break;
				};
		}
	
};

int main(int argc, char* argv[])
{
	int tSeconds = 60;
	
	//Config Args
	if(argc > 1){
		if (isdigit(std::stoi(argv[1])) == false){
			tSeconds = std::stoi(argv[1]);
		}
	}
	
	initscr();
	curs_set(0);
	int y;
	int x;
	getmaxyx(stdscr, y,x);
	if(x < 80 || y < 20){//Check terminal size
		endwin();
		std::cout << "Terminal Window is not big enough." << std::endl;
	}else{
			
	
		nodelay(stdscr, TRUE);

		Farm farm;
		
		Timer timer(tSeconds);


		while(timer.checkHitTime(std::chrono::steady_clock::now()) == false){
			
			
						
			farm.update(timer);
			farm.draw_all(x);
			mvprintw(1,1,"%s","Press any key to quit...");
			
			//If something is pressed the counting ends
			if(getch() != -1){
				break;
			}

			refresh();

		}
        
		endwin();
		
	}

    return EXIT_SUCCESS;
}
