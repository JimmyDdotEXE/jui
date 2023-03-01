#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>


typedef unsigned int uint;
typedef unsigned char uchar;


double degreesToRadians(double d);
double radiansToDegrees(double r);

bool isNumeric(std::string s);
bool isDirectory(std::string s);

std::string numString(int num);
int stringInt(std::string s);
std::string stringLower(std::string s);

bool sortAlpha(std::vector<std::string> *v);

#endif
