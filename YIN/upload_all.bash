#!/bin/bash

# 0/10 would not recommend this script do not use for reference


sed -i -e 's#AUTO_TIME_LIMIT true#AUTO_TIME_LIMIT false#' ./src/main.cpp

# pros upload --machine-output

# defaults
function set_defaults()
{
    sed -i -e 's#"project_name": .*,#"project_name": "YIN blue negative",#' project.pros
    sed -i -e 's#"slot": .*#"slot": "1"#' project.pros
    sed -i -e 's#RECORD .*#RECORD false#' ./src/main.cpp
    sed -i -e 's#isBlue = .*;#isBlue = true;#' ./src/main.cpp
    sed -i -e "s#side = .*;#side = \'N\';#" ./src/main.cpp
    sed -i -e "s#start_sorting = .*;#start_sorting = true;#" ./src/main.cpp
}

# reset when ending early
trap exit_command INT
function exit_command()
{
    set_defaults
    exit 1
}


echo "START UPLOAD AT: $(date +"%d/%m/%y %T")"

# blue negative
set_defaults
pros make
pros upload
echo "BLUE NEGATIVE FINISHED"

# blue center
sed -i -e 's#"project_name": .*,#"project_name": "YIN blue center",#' project.pros
sed -i -e 's#"slot": .*#"slot": "2"#' project.pros
sed -i -e 's#RECORD .*#RECORD false#' ./src/main.cpp
sed -i -e 's#isBlue = .*;#isBlue = true;#' ./src/main.cpp
sed -i -e "s#side = .*;#side = \'C\';#" ./src/main.cpp
sed -i -e "s#start_sorting = .*;#start_sorting = true;#" ./src/main.cpp
pros make
pros upload
echo "BLUE CENTER FINISHED"

# red negative
sed -i -e 's#"project_name": .*,#"project_name": "YIN red negative",#' project.pros
sed -i -e 's#"slot": .*#"slot": "3"#' project.pros
sed -i -e 's#RECORD .*#RECORD false#' ./src/main.cpp
sed -i -e 's#isBlue = .*;#isBlue = false;#' ./src/main.cpp
sed -i -e "s#side = .*;#side = \'N\';#" ./src/main.cpp
sed -i -e "s#start_sorting = .*;#start_sorting = true;#" ./src/main.cpp
pros make
pros upload
echo "RED NEGATIVE FINISHED"

# red center
sed -i -e 's#"project_name": .*,#"project_name": "YIN red center",#' project.pros
sed -i -e 's#"slot": .*#"slot": "4"#' project.pros
sed -i -e 's#RECORD .*#RECORD false#' ./src/main.cpp
sed -i -e 's#isBlue = .*;#isBlue = false;#' ./src/main.cpp
sed -i -e "s#side = .*;#side = \'C\';#" ./src/main.cpp
sed -i -e "s#start_sorting = .*;#start_sorting = true;#" ./src/main.cpp
pros make
pros upload
echo "RED CENTER FINISHED"

# skills
sed -i -e 's#"project_name": .*,#"project_name": "YIN driver skills",#' project.pros
sed -i -e 's#"slot": .*#"slot": "5"#' project.pros
sed -i -e 's#RECORD .*#RECORD false#' ./src/main.cpp
sed -i -e 's#isBlue = .*;#isBlue = false;#' ./src/main.cpp
sed -i -e "s#side = .*;#side = \'N\';#" ./src/main.cpp
sed -i -e "s#start_sorting = .*;#start_sorting = false;#" ./src/main.cpp
pros make
pros upload
echo "SKILLS FINISHED"

# record
sed -i -e 's#"project_name": .*,#"project_name": "YIN record",#' project.pros
sed -i -e 's#"slot": .*#"slot": "6"#' project.pros
sed -i -e 's#RECORD .*#RECORD true#' ./src/main.cpp
sed -i -e 's#isBlue = .*;#isBlue = false;#' ./src/main.cpp
sed -i -e "s#side = .*;#side = \'N\';#" ./src/main.cpp
sed -i -e "s#start_sorting = .*;#start_sorting = false;#" ./src/main.cpp
pros make
pros upload
echo "RECORD FINISHED"

echo "FINISH UPLOAD AT: $(date +"%d/%m/%y %T")"
set_defaults

