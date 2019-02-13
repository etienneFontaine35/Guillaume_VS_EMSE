#! /bin/bash


g++  -std=c++11 -Wall Objet_Volant.cpp Tools.cpp Background.cpp Menu.cpp main.cpp -o Guillaume_VS_EMSE -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system -lboost_serialization
# g++ -g -std=c++11 -Wall Objet_Volant.cpp Tools.cpp Background.cpp Menu.cpp main.cpp -o Game -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system -lboost_serialization

# g++  -std=c++11 -Wall Tools.cpp test.cpp -o essai -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system -lboost_serialization
