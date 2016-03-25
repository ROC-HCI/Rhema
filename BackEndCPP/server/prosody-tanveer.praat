##############################################
####### Coded by M. Iftekhar Tanveer #########
############ go2chayan@gmail.com  ############
##############################################
form this form gets command line text arguments
word filename "somename"
endform
#echo the command line arguments were 'filename$'

if (fileReadable (filename$))
#echo file 'filename$' is readable
else
#echo file 'filename$' is not readable
endif

Read from file... 'filename$'

soundname$ = selected$("Sound")

################## Extract Pitch ##########################
To Pitch (ac)... 0 75 15 no 0.03 0.8 0.01 0.35 0.14 400
select Pitch 'soundname$'
To Matrix
select Matrix 'soundname$'
Save as headerless spreadsheet file: "output.pitch"
select Matrix 'soundname$'
Remove

#################### Intensity Analysis ###################
select Sound 'soundname$'
To Intensity... 100 0.01 yes
Down to Matrix
select Matrix 'soundname$'
Save as headerless spreadsheet file: "output.loud"