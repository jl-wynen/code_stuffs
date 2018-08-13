#!/usr/bin/python
#
# Print a barplot using unicode
#

import random
import shutil

def utf_bar_plot(data, labels=None, labels_length_max=10, numbers_format="{:f}",
                 absolute_numbers=True, ncols=-1):

    # unicode bars elements for the plot
    blocks = [chr(0x258F-i) for i in range(0, 8)]

    # get width of stdout if needed
    if ncols == -1:
        ncols = shutil.get_terminal_size().columns
    
    data_max = max(data)

    # turn into relative numbers for data and create numbers texts
    if absolute_numbers:
        numbers_strs = [numbers_format.format(n) for n in data]
    data = [e/data_max for e in data]
    if not absolute_numbers:
        numbers_strs = [numbers_format.format(n) for n in data]
    
    # get length of displayed numbers and pad shorter ones
    numbers_length = max(map(len, numbers_strs))
    for i in range(len(numbers_strs)):
        numbers_strs[i] = " "*(numbers_length-len(numbers_strs[i])) + numbers_strs[i]

    if not labels:
        labels = [""]*len(data)
    # crop and tag labels if needed
    if labels_length_max == -1:
        labels_length_max = ncols//2

    labels_full = []
    labels_length = 0
    cnt = 0
    for i in range(len(labels)):
        if len(labels[i]) > labels_length_max:
            cnt += 1
            labels_full.append(labels[i])
            labels_length = labels_length_max
        
            if cnt < 10:  # one digit tag
                labels[i] = labels[i][:max(labels_length_max-3, 0)]+"[{:1d}]".format(cnt)
            else:  # two digits (anything more is crazy)
                labels[i] = labels[i][:max(labels_length_max-4, 0)]+"[{:2d}]".format(cnt)
        else:
            if len(labels[i]) > labels_length:
                labels_length = len(labels[i])
    if cnt < 10 and labels_length < 3:
        labels_length = 3
    elif cnt >= 10 and labels_length < 4:
        labels_length = 4
                
    labels = [" "*(labels_length-len(l)) + l for l in labels]
    
    #subtract widths of labels and numbers (2 for |'s)
    bars_length_max = ncols - numbers_length - 2 - labels_length

    # get number of full blocks and rest
    blengths = [(int((e*bars_length_max*8)//8), int((e*bars_length_max*8)%8))
                for e in data]
    
    # build bars
    bars = []
    for l in blengths:
        if (l[1]) == 0:  # do not append anything when we have no remainder
            bars.append(blocks[-1]*l[0])
        else:
            bars.append(blocks[-1]*l[0]+blocks[l[1]])

    # write plot string
    if (labels_length != 0):
        plot_str = " "*(labels_length-5) + "Label "
    else:
        plot_str = ""
    if absolute_numbers:
        plot_str += " "*(ncols-max(labels_length, 5)-1-19) + "Quantity (absolute)\n"
    else:
        plot_str += " "*(ncols-max(labels_length, 5)-1-19) + "Quantity (relative)\n"
    
    for i in range(len(data)):
        plot_str += labels[i] + "|" + bars[i] + " "*(bars_length_max-len(bars[i])) \
                    + "|" + numbers_strs[i]
        if i != len(data) - 1:  # add newline only for all but last
            plot_str += "\n"

    if len(labels_full) != 0:
        plot_str += "\n\n"
    for i in range(len(labels_full)):
        if (i+1) < 10:
            plot_str += "[{:1d}] ".format(i+1) + labels_full[i]
        else:
            plot_str += "[{:2d}] ".format(i+1) + labels_full[i]
        
        if i != len(labels_full) - 1:  # add newline only for all but last
            plot_str += "\n"


    return plot_str



# data = [1, 2, 3, 4, 10, 50, 32, 21, 32, 0, 1]

data = [random.random() for i in range(50)]

# labels = ["ABC", "Something important", "", "even more, terribly important stuff"]

labels = [str(i) for i in range(1, len(data)+1)]
labels_length_max = 10

absolute_numbers = True
numbers_format = "{:.2f}"

print(utf_bar_plot(data, labels=labels, labels_length_max=labels_length_max,
                   absolute_numbers=absolute_numbers,
                   numbers_format=numbers_format, ncols=-1))
