/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.
 *
 *   \file   sampleplugin.h
 *   \brief  Script engine plugins
 *   \author Greg Smith
 *   \date   08/05/2019 -  Created
 */

#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include "uxpscript.h"

namespace uxpplugin01
{
  class plugin01 : public uxp::scriptPlugin
  {
    public:
      plugin01() {}

      ~plugin01() {}

      /**
       * @brief Gets list of functions
       * @return List of functions
       */
      std::vector<uxp::scriptFunction *> getFunctionList();

      /**
       * @brief Gets a function descriptors
       * @param name Function name
       * @return Script function
       */
      uxp::scriptFunction *getFunction(const std::string &name);
  };

  // Exporting `uxpplugin01::plugin` variable with alias name `uxp_scriptplugin`
  extern "C" UXPLB plugin01 uxp_scriptplugin;

  plugin01 uxp_scriptplugin;
}

#endif // SAMPLEPLUGIN_H
