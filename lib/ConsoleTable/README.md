# ConsoleTable
ConsoleTable is a library that allows you to organize your data in a table based structure.

## Overview

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Sometimes, it can be very frustrating to display data in a console with proper alignment. 
To make this easier, ConsoleTable was created. It is a customizable text based table structure which is really convenient to use.
It allows you to organize your data in a table structure where you can easily add, update and delete rows.
Furthermore, it comes with sorting functionality as well as different layouts the user can choose from.
This is an example how the table can look like:
```
+---------------+-------------+-----------------+-----------------+---------------------+
|  Country      |  Capital    |  Population     |  Area           |  Currency           |
+---------------+-------------+-----------------+-----------------+---------------------+
|  Australia    |  NEW ENTRY  |  24,877,800     |  7,692,024 km2  |  Australian Dollar  |
|  China        |  Beijing    |  1,403,500,365  |  9,596,961 km2  |  Yuan               |
|  France       |  Paris      |  67,201,000     |  640,679 km2    |  Euro               |
|  Germany      |  Berlin     |  82,800,000     |  357,168 km2    |  Euro               |
|  Iceland      |  Reykjavik  |  348,580        |  102,775 km2    |  Icelandic Krona    |
|  Netherlands  |  Amsterdam  |  17,200,671     |  41,543 km2     |  Euro               |
+---------------+-------------+-----------------+-----------------+---------------------+
```

## Usage
To create a new table, a `ConsoleTable` object must be initialized. The constructor expects a `std::initializer_list<std::string>`, which resembles the table headers.
```cpp
ConsoleTable table{"Country", "Capital", "Population", "Area", "Currency"};
```
Next, the padding size must be set. The padding size defines the spacing between the text in each cell of the table and the border of the cell. Default value is `1`.
The padding size can be set as follows
```cpp
table.setPadding(2); // Sets a padding of 2 for each cell
```
The user can choose between a total of four different table layouts. `0` is the default style. `1` is a single, `2` a double outlined style and `3` an invisible table styles with no lines at all. To change the style call
```cpp
table.setStyle(0); // Sets the default table style
```
It's important to note that some styles might not be displayed correctly if your system doesn't support special characters. 
The default style `0` as well as style `3` should work on all systems because only ASCII characters are used for the layout.

To add new rows of data to the table use the `+=` operator followed by a list of `strings`.
```cpp
table += {"Germany", "Berlin", "82,800,000", "357,168 km2", "Euro"};
table += {"France", "Paris", "67,201,000", "640,679 km2 ", "Euro"};
table += {"South Korea", "Seoul", "51,446,201", "100,210 km2 ", "South Korean Won"};
table += {"Australia", "Canberra", "24,877,800", "7,692,024 km2", "Australian Dollar"};
table += {"China", "Beijing", "1,403,500,365", "9,596,961 km2", "Yuan"};
table += {"Iceland", "Reykjavik", "348,580", "102,775 km2", "Icelandic Krona"};
table += {"Netherlands", "Amsterdam", "17,200,671", "41,543 km2", "Euro"};
```
To change the update the value of a specific cell in the console table use the `updateRow(int, int, string)` function.
The first parameter is the index of the row, the second one the index of the column that should be updated. The last parameter is the new `string` that should be set.
```cpp
table.updateRow(3, 1, "NEW ENTRY"); // Update row 3, column 1
```
The same thing applies for the headers. If you want to change the text of one of the header fields, simply provide the index of the field you like to update and call
```cpp
table.updateHeader(3, "NEW CAPTION"); // Update header field at index 3 
```
To output the table to the console, simply use the insertion operator followed by the `ConsoleTable` object.
```cpp
std::cout << table; // Display table in the console
```

It is also possible to sort the table based on the first column. To do that, call
```cpp
table.sort(true); // Sort ascending
table.sort(false); // Sort descending
```
To remove rows from the table use the `-=` operator followed by the index of the row that should be removed.
```cpp
table -= 2; // Removes row at index 2 from the table
```

## Requirements
This library requires C++11 to compile.
