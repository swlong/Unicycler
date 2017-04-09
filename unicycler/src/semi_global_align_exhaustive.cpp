// Copyright 2017 Ryan Wick (rrwick@gmail.com)
// https://github.com/rrwick/Unicycler

// This file is part of Unicycler. Unicycler is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version. Unicycler is
// distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details. You should have received a copy of the GNU General Public
// License along with Unicycler. If not, see <http://www.gnu.org/licenses/>.

#include "semi_global_align_exhaustive.h"

#include <seqan/align.h>
#include "semi_global_align.h"



char * semiGlobalAlignmentExhaustive(char * s1, char * s2,
                                     int matchScore, int mismatchScore,
                                     int gapOpenScore, int gapExtensionScore) {

    // Change the sequences to C++ strings.
    std::string sequence1(s1);
    std::string sequence2(s2);

    ScoredAlignment * alignment = semiGlobalAlignmentExhaustive(sequence1, sequence2,
                                                                matchScore, mismatchScore,
                                                                gapOpenScore, gapExtensionScore);
    if (alignment != 0) {
        std::string returnString = alignment->getFullString();
        delete alignment;
        return cppStringToCString(returnString);
    }
    else
        return cppStringToCString("");
}


ScoredAlignment * semiGlobalAlignmentExhaustive(std::string s1, std::string s2,
                                                int matchScore, int mismatchScore,
                                                int gapOpenScore, int gapExtensionScore) {
    long long startTime = getTime();

    Dna5String sequenceH(s1);
    Dna5String sequenceV(s2);

    Align<Dna5String, ArrayGaps> alignment;
    resize(rows(alignment), 2);
    assignSource(row(alignment, 0), sequenceH);
    assignSource(row(alignment, 1), sequenceV);
    Score<int, Simple> scoringScheme(matchScore, mismatchScore, gapExtensionScore, gapOpenScore);

    AlignConfig<true, true, true, true> alignConfig;
    try {
        globalAlignment(alignment, scoringScheme, alignConfig);
    }
    catch (...) {
        return 0;
    }

    std::string s1Name = "s1";
    std::string s2Name = "s2";

    return new ScoredAlignment(alignment, s1Name, s2Name, s1.length(), s2.length(),
                               0, startTime, 0, true, true, true, scoringScheme);
}


