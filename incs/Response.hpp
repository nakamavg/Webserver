#pragma once
#include "Libs.hpp"

class Response
{
  public:
	Response(std::string &html);
	~Response();
	std::string get_web();

  private:
	std::string web;
};