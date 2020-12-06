#include <iostream>

void userInterfaceButtons() {
    std::string row1[4] = { "( "," )","CE","C  " };
    std::string row2[4] = { "%  ","y^x","r(x)","/" };
    std::string row3[4] = { "7","8","9","*" };
    std::string row4[4] = { "4","5","6","-" };
    std::string row5[4] = { "1","2","3","+" };
    std::string row6[4] = { "OFF","0",".","=" };
    
    std::cout<<"|  ";
    for(int i = 0; i < 4; i++){
        if(i == 1)
            std::cout << row1[i] << " |  ";
        else if(i == 3)
            std::cout << row1[i] << "|";
        else 
            std::cout<<row1[i] << " |  ";
    }
    std::cout << std::endl;
    
    std::cout << "| ";
    for(int i = 0; i < 4; i++){
        if(i == 1)
            std::cout << row2[i] << " | ";
        else if(i==2)
            std::cout << row2[i] << "|   ";
        else
            std::cout<<row2[i]<<" | ";
    }
    std::cout << std::endl;
    
    std::cout << "|  ";
    for(int i = 0; i < 4; i++)
        std::cout << row3[i] << "  |  ";
    std::cout << std::endl;
    
    std::cout << "|  ";
    for(int i = 0; i < 4; i++)
        std::cout << row4[i] << "  |  ";
    std::cout << std::endl;
    
    std::cout << "|  ";
    for(int i = 0; i < 4; i++) 
        std::cout << row5[i] << "  |  ";
    std::cout << std::endl;
    
    std::cout << "| ";
    for(int i = 0; i < 4; i++){
        if(i == 0)
            std::cout<<row6[i] << " |  ";
        else    
           std::cout<<row6[i] << "  |  ";
    }
    std::cout<<std::endl;
}

void userInterfaceDisplay(std::string element){
    char top[25] = { '_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_' };
    char bot[25] = { '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-' };
    char screen[23]={ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    
    for(int i = 0; i < 25; i++)
        std::cout << top[i];
    
    std::cout << std::endl <<"|";
    for (int i = 0; i < 23; i++){
        for(int j = 0; j < element.length(); j++)
            screen[22 - j] = element[(element.length() - 1) - j];
        std::cout << screen[i];
    }
    std::cout << "|" << std::endl;

    std::cout << "|";
    for(int i = 0; i < 23; i++)
        std::cout << bot[i];
    std::cout << "|" << std::endl;
    
    userInterfaceButtons();
}
