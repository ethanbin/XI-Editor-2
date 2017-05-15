//  Created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** Listing 7-6.
@file NotFoundException.cpp */
#include "NotFoundException.h"  

NotFoundException::NotFoundException(const std::string& message)
	: std::logic_error("NotFoundException Violated Exception: " + message)
{
}  // end constructor

// End of implementation file.

