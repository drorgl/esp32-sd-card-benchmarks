#include "benchmark_printout.h"
#include <string>

#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>

template<class T>
std::string FormatWithCommas(T value)
{
    std::string temp = std::to_string((int64_t)value);

    unsigned int numLength = temp.size();

    temp.reserve((numLength - 1) / 3 + numLength);
    for (unsigned int i = 1; i <= (numLength - 1) / 3; i++)
        temp.insert(numLength - i * 3, 1, ',');
    
    return temp;

    // for (auto i = 0; i < number.length(); i++){
    //     if (((i % 3) == 0) && (i != 0) && (i != number.length())){
    //         number.insert(number.length() - i,",");            
    //     }
    // }
    // return number;
}

std::string humanSize(uint64_t bytes)
{
	char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
	char length = sizeof(suffix) / sizeof(suffix[0]);

	int i = 0;
	double dblBytes = bytes;

	if (bytes > 1024) {
		for (i = 0; (bytes / 1024) > 0 && i<length-1; i++, bytes /= 1024)
			dblBytes = bytes / 1024.0;
	}

    return FormatWithCommas(dblBytes) + " " + suffix[i];
	// static char output[200];
	// sprintf(output, "%.02lf %s", dblBytes, suffix[i]);
	// return output;
}





void print(struct statistics &stats)
{
    printf("%s%s%s%s block %" PRIu32 " time %" PRIu64 " ms, iops %" PRIu64 ", bytes %" PRIu64 ", %" PRIu64 " b/s",
           stats.rw ? " write" : " read",
           stats.random ? " random" : " sequential",
           stats.sync ? " sync" : "",
           stats.aligned ? " aligned " : "",
           stats.block_size,
           stats.time,
           (uint64_t)((double)stats.iops / (double)(stats.time / 1000.0)),
           stats.bytes,
           (uint64_t)((double)stats.bytes / (double)(stats.time / 1000.0)));
}


void addTable(ConsoleTable &table, struct statistics &stats){
    auto blockSize = FormatWithCommas(stats.block_size);
    auto rw = stats.rw ? "W" : "R";
    auto random = stats.random ? "Rnd" : "Seq";
    auto sync = stats.sync ? "V" : "X";
    auto aligned = stats.aligned ? FormatWithCommas(stats.aligned) : "Not";
    auto time = FormatWithCommas(stats.time) + "ms";
    auto iops = FormatWithCommas((uint64_t)((double)stats.iops / (double)(stats.time / 1000.0)));
    auto bytes = humanSize(stats.bytes);
    auto bps = FormatWithCommas((uint64_t)((double)stats.bytes / (double)(stats.time / 1000.0))) ;
    table.addRow({blockSize, rw, random, sync, aligned, time, iops, bytes, bps});
}