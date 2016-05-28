INCLUDE = -I../
FLAGS = -Wl,--no-as-needed -std=c++11 -lpthread
EXAMPLES = Dice_Simulator Flashing_Character Greedy_Snake Hello_CGL Multi_Window_Marble \
Progress_Bar Test_Basic_Screen Test_Button_Handle Test_Console Test_Multi_Window \
Test_Random_Fill Test_Random_Move Text_Editor

all: $(EXAMPLES)

$(EXAMPLES): %: %.cpp
	g++ $(FLAGS) $(INCLUDE) $< -o $@

PHONY: clean  
   
clean :
	rm $(EXAMPLES)
