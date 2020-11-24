#pragma once
#include "Procedures.h"

namespace preppy::procedures {

    class Vivification
        : public preppy::procedures::Procedure {

    public:

        bool apply(cnf::CNF& formula) override;

    protected:


    private:

    };

}
