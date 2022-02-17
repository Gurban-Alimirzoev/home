from pathlib import Path    

listAntEl = [
    '1_2.cti',
    '1_3.cti',
    '1_4.cti',
    '1_5.cti',
    '1_6.cti',
    '2_3.cti',
    '2_4.cti',
    '2_5.cti',
    '2_6.cti',
    '3_4.cti',
    '3_5.cti',
    '3_6.cti',
    '4_5.cti',
    '4_6.cti',
    '5_6.cti'
]

home = Path.cwd()
#for name in listAntEl:
dateDict = {() : {():()} }

for name in listAntEl:
    file = open(home / name)
    oneOne = file.readlines()[314:615]
    file = open(home / name)
    oneTwo = file.readlines()[617:918]
    file = open(home / name)
    twoOne = file.readlines()[920:1221]
    file = open(home / name)
    twoTwo = file.readlines()[1223:1524]
    dateDict.update({name: {'1_1': oneOne,
    '1_2' : oneTwo,
    '2_1' : twoOne,
    '2_2' : twoTwo}})

endString = 'END\n'
beginString = 'BEGIN\n'
antennaName = 'antenna.cit'

file = open(home / antennaName)
lines = file.readlines()[:]
beginOfAnt = lines[0 : 348]
endOfAnt = lines[11256 : ]

with open(home / antennaName, 'w') as file:
    file.writelines(beginOfAnt)

    file.writelines(beginString)
    file.writelines(dateDict['1_2.cti']['1_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_2.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_3.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_4.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_5.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_6.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_2.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_2.cti']['2_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_3.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_4.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_5.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_6.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_3.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_3.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_3.cti']['2_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_4.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_5.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_6.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_4.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_4.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_4.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_4.cti']['2_2'])
    file.writelines(endString)

    file.writelines(beginString)
    file.writelines(dateDict['4_5.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['4_6.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_5.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_5.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_5.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['4_5.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['4_5.cti']['2_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['5_6.cti']['1_2'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['1_6.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['2_6.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['3_6.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['4_6.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['5_6.cti']['2_1'])
    file.writelines(endString)
    
    file.writelines(beginString)
    file.writelines(dateDict['5_6.cti']['2_2'])
    file.writelines(endString)    
    
    file.writelines(endOfAnt)