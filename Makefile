
bascom: bascom.cc
	c++ -g bascom.cc `llvm-config --cxxflags --ldflags` -o bascom `llvm-config --libs` -lpthread -lncurses -ldl
