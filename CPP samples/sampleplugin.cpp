/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.
 *
 *   \file   sampleplugin.cpp
 *   \brief  Script engine plugins
 *   \author Greg Smith
 *   \date   08/05/2019 -  Created
 */

#include "sampleplugin.h"

#include <map>

static void init();

static std::map<std::string, uxp::scriptFunction *> m_names;

/**
 * @brief Gets list of functions
 * @return List of functions
 */
std::vector<uxp::scriptFunction *> uxpplugin01::plugin01::getFunctionList()
{
  std::vector<uxp::scriptFunction *> list;

  init();

  for (auto &p: m_names)
  {
    list.push_back(p.second);
  }

  return list;
}

/**
 * @brief Gets a function descriptors
 * @param name Function name
 * @return Script function
 */
uxp::scriptFunction *uxpplugin01::plugin01::getFunction(const std::string &name)
{
  init();

  if (m_names.find(name) != m_names.end())
    return m_names[name];

  return nullptr;
}

class UXPLB_HIDDEN helloWorld : public uxp::scriptFunction
{
  public:

    helloWorld() :  uxp::scriptFunction ("greg", "greg::helloWorld", 0, 0) {}

    virtual ~helloWorld() {}

    /**
     * @brief Function body.  Is executed when the script function is called by the user.
     */
    virtual void execute(void)
    {
      std::cout << "Hello, world!" << std::endl;

      setReturn(std::string("Or not"));
    }

  private:
};

class UXPLB_HIDDEN printArgs : public uxp::scriptFunction
{
  public:

    printArgs() :  uxp::scriptFunction ("greg", "greg::printArgs", 0, 5) {}

    virtual ~printArgs() {}

    /**
     * @brief Function body.  Is executed when the script function is called by the user.
     */
    virtual void execute(void)
    {
      std::cout << "Arg count: " << getArgCount() << std::endl;

      for (int i = 0; i < getArgCount(); i++)
      {
        std::cout << "Arg " << i+1 << ": " << getArgument(i).toString() << std::endl;
      }

      setReturn(true);
    }

  private:
};

static void init()
{
  if (m_names.empty())
  {
    uxp::scriptFunction *ptr = new helloWorld();

    m_names[ptr->getName()] = ptr;

    ptr = new printArgs();
    m_names[ptr->getName()] = ptr;
  }
}



