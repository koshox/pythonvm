//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_BINARYFILEPARSER_HPP
#define PYTHONVM_BINARYFILEPARSER_HPP

#include "../util/bufferedInputStream.hpp"
#include "../util/arrayList.hpp"
#include "../object/hiString.hpp"
#include "codeObject.hpp"

class BinaryFileParser {
private:
    BufferedInputStream *file_stream;

    int cur;

    ArrayList<HiString *> _string_table;

public:
    BinaryFileParser(BufferedInputStream *stream);

public:
    CodeObject *parse();

    CodeObject *get_code_object();

    HiString *get_byte_codes();

    HiString *get_no_table();

    HiString *get_string();

    HiString *get_name();

    HiString *get_file_name();

    ArrayList<HiObject *> *get_consts();

    ArrayList<HiObject *> *get_names();

    ArrayList<HiObject *> *get_var_names();

    ArrayList<HiObject *> *get_free_vars();

    ArrayList<HiObject *> *get_cell_vars();

    ArrayList<HiObject *> *get_tuple();
};

#endif //PYTHONVM_BINARYFILEPARSER_HPP
