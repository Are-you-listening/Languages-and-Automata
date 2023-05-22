//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WDFA_H
#define WNFA_WDFA_H

#include "WNFA.h"

using namespace std;
using json = nlohmann::json;

class WDFA: public WNFA {
public:

    /**
     * \brief Empty Constructor
     */
    WDFA();

    /**
     * \brief Constructor from a File
     * @param filename
     */
    explicit WDFA(const string &filename);

    /**
     * \brief Print the object to the terminal
     */
    void print() const override;

    /**
     * \brief Check if a certain string is accepted and returns its weight
     * @param input
     * @return
     */
    [[nodiscard]] double weightedaccepts(const string &input) const override;
};

#endif //WNFA_WDFA_H
