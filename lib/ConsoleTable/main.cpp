#include <iostream>
#include "ConsoleTable.h"

int main() {

    ConsoleTable table{"Country", "Capital", "Population", "Area", "Currency"};

    table.setPadding(2);
    table.setStyle(0);

    table += {"Germany", "Berlin", "82,800,000", "357,168 km2", "Euro"};
    table += {"France", "Paris", "67,201,000", "640,679 km2 ", "Euro"};
    table += {"South Korea", "Seoul", "51,446,201", "100,210 km2 ", "South Korean Won"};
    table += {"Australia", "Canberra", "24,877,800", "7,692,024 km2", "Australian Dollar"};
    table += {"China", "Beijing", "1,403,500,365", "9,596,961 km2", "Yuan"};
    table += {"Iceland", "Reykjavik", "348,580", "102,775 km2", "Icelandic Krona"};
    table += {"Netherlands", "Amsterdam", "17,200,671", "41,543 km2", "Euro"};

    table.updateRow(3, 1, "NEW ENTRY");
    table.updateHeader(2, "NEW HEADER");

    table -= 2;
    table -= 1;
    table -= 0;
    table.sort(true);

    std::cout << table;

    return 0;
}