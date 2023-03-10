import matplotlib.pyplot as plt
import numpy as np
from math import sqrt
import matplotlib.lines as lines
import matplotlib.transforms as mtransforms
import matplotlib.text as mtext
import random
import sys


def dis(x1, y1, x2, y2):
    return abs(x1-x2)+abs(y1-y2)


def fp_plot(arrange, FL, filename_pin, filename_bp_route, filename_2pin_nets_route, filename_3pin_net_res, fig_filename):

    fig = plt.figure(figsize=(20, 20))
    ax = plt.subplot(1, 1, 1)

    fp = open(filename_pin, "r")
    floorplane = plt.Rectangle(
        (0, 0), FL[0], FL[1], edgecolor='black', fill=False, linewidth=2)
    ax.add_patch(floorplane)
    chipdraw = plt.Rectangle((290, 473), 520, 474, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((215, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((615, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)

    offset_w = FL[0] - 160
    offset_h = FL[1] - 160
    offset_boundary = plt.Rectangle(
        (80, 80), offset_w, offset_h, edgecolor='black', fill=False, linewidth=2, linestyle='dashed',)
    txt = 'Offset_boundary ' + str(offset_w)+" x "+str(offset_h)
    plt.text(x=80, y=70, s=txt, fontsize=10, fontstyle='oblique')
    ax.add_patch(offset_boundary)

    c = 0
    line = fp.readline()
    while line != '':

        pos = line.split()
        x = float(pos[0])
        y = float(pos[1])
        w = 10.0
        h = 10.0
        name = "("+str(x)+",\n"+str(y) + ")"
        if (c >= 85+31+31):
            pad = plt.Rectangle((x-25, y-25), 50, 50, edgecolor='black',
                                facecolor='lightyellow', fill=True, linewidth=2)
            ax.add_patch(pad)
        c += 1
        # pin = plt.Rectangle((x-w/2, y-w/2), w, h, edgecolor='black',
        #                     facecolor='blue', fill=True, linewidth=2)
        # ax.add_patch(pin)
        #plt.text(x=x+int(w/2), y=y+int(h/2), s=name, fontsize=5, fontstyle='oblique')

        line = fp.readline()
    fp.close()
    # draw transparent lines with width 10
    # fp2 = open(filename_bp_route, "r")
    # line2 = fp2.readline()
    # while line2 != '':
    #     for i in range(3):
    #         line2 = fp2.readline()
    #         # print(line2)
    #         pos = line2.split()
    #         x1 = float(pos[0])
    #         y1 = float(pos[1])
    #         x2 = float(pos[3])
    #         y2 = float(pos[4])
    #         ax.plot([x1, x2], [y1, y2], linewidth=10,
    #                 color='lightgreen', alpha=0.8)
    #     line2 = fp2.readline()
    # fp2.close()

    # draw black mid-lines with width 1
    # fp2 = open(filename_bp_route, "r")
    # line2 = fp2.readline()
    # while line2 != '':
    #     for i in range(3):
    #         line2 = fp2.readline()
    #         # print(line2)
    #         pos = line2.split()
    #         x1 = float(pos[0])
    #         y1 = float(pos[1])
    #         x2 = float(pos[3])
    #         y2 = float(pos[4])
    #         ax.plot([x1, x2], [y1, y2], linewidth=1, color='black')
    #     line2 = fp2.readline()
    # fp2.close()

    # draw 3-pin net resource used
    fp = open(filename_3pin_net_res, "r")
    line = fp.readline()

    flag = False
    while line != '':
        pos = line.split()
        if(len(pos) < 5):
            flag = True
            netnum = "m" + pos[3]
            line = fp.readline()
            pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 0):
                x = float(pos[i][1:-1])
            elif(i % 4 == 1):
                y = float(pos[i][0:-1])
            elif(i % 4 == 2):
                h = float(pos[i][0:-1])
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='red', alpha=0.8)
                elif (prev_h == 2 and h == 2):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='blue', alpha=0.8)
                elif (prev_h == 3 and h == 3):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='green', alpha=0.8)

                if (x == prev_x and y == prev_y and abs(h-prev_h) == 1):
                    color = ['lightyellow', 'pink', 'lightblue', 'lightgreen']
                    tile_used = plt.Rectangle(
                        (x-7, y-7), 14, 14, edgecolor='black', facecolor='lightyellow', fill=True, linewidth=1, alpha=0.8)
                    ax.add_patch(tile_used)
            if(i == 3 or (i == (len(pos)-1) and flag)):
                plt.text(x=x-4, y=y, s=netnum,
                         fontsize=10, fontstyle='oblique')
            prev_x = x
            prev_y = y
            prev_h = h
        flag = False
        line = fp.readline()
    fp.close()

    # draw 2-pin net routing paths...RDL1
    fp = open(filename_2pin_nets_route, "r")
    line = fp.readline()
    count = 0
    while line != '':
        pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if (i == 0):
                pathname = pos[i][4:-1]
            if(i % 4 == 1):
                x = float(pos[i][1:-1])
                # print(x)
            elif(i % 4 == 2):
                y = float(pos[i][0:-1])
                # print(y)
            elif(i % 4 == 3):
                h = float(pos[i][0:-1])
                # print(h)
            if(i == 3):
                plt.text(x=x-10, y=y, s=pathname,
                         fontsize=10, fontstyle='oblique')
            elif(i == len(pos)-1):
                plt.text(x=x-10, y=y, s=pathname,
                         fontsize=10, fontstyle='oblique')
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='red', alpha=0.8)
#                 elif (prev_h == 2 and h ==2):
#                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
#                 elif (prev_h == 3 and h ==3):
#                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
#                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
                    #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
#                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
#                     ax.add_patch(via)
#                     count+=1

            prev_x = x
            prev_y = y
            prev_h = h
        line = fp.readline()
        line = fp.readline()
    fp.close()

    # draw 2-pin net routing paths...RDL2
    fp = open(filename_2pin_nets_route, "r")
    line = fp.readline()
    count = 0
    while line != '':
        pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 1):
                x = float(pos[i][1:-1])
                # print(x)
            elif(i % 4 == 2):
                y = float(pos[i][0:-1])
                # print(y)
            elif(i % 4 == 3):
                h = float(pos[i][0:-1])
                # print(h)
            if(prev_x != -1 and prev_y != -1):
                #                 if (prev_h == 1 and h ==1):
                #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                if (prev_h == 2 and h == 2):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='blue', alpha=0.8)
#                 elif (prev_h == 3 and h ==3):
#                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
#                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
                    #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
#                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
#                     ax.add_patch(via)
#                     count+=1

            prev_x = x
            prev_y = y
            prev_h = h
        line = fp.readline()
        line = fp.readline()
    fp.close()

    # draw 2-pin net routing paths...RDL3
    fp = open(filename_2pin_nets_route, "r")
    line = fp.readline()
    count = 0
    while line != '':
        pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 1):
                x = float(pos[i][1:-1])
                # print(x)
            elif(i % 4 == 2):
                y = float(pos[i][0:-1])
                # print(y)
            elif(i % 4 == 3):
                h = float(pos[i][0:-1])
                # print(h)
            if(prev_x != -1 and prev_y != -1):
                #                 if (prev_h == 1 and h ==1):
                #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                #                 elif (prev_h == 2 and h ==2):
                #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
                if (prev_h == 3 and h == 3):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='green', alpha=0.8)
#                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
#                     #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
#                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
#                     ax.add_patch(via)
#                     count+=1

            prev_x = x
            prev_y = y
            prev_h = h
        line = fp.readline()
        line = fp.readline()
    fp.close()

    # draw 2-pin net routing paths...VIA
    fp = open(filename_2pin_nets_route, "r")
    line = fp.readline()
    count = 0
    while line != '':
        pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 1):
                x = float(pos[i][1:-1])
                # print(x)
            elif(i % 4 == 2):
                y = float(pos[i][0:-1])
                # print(y)
            elif(i % 4 == 3):
                h = float(pos[i][0:-1])
                # print(h)
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                    a = 1
                elif (prev_h == 2 and h == 2):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
                    a = 1
                elif (prev_h == 3 and h == 3):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
                    a = 1
                elif (prev_x == x and prev_y == y and prev_h != -1):  # via usage
                    #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
                    via = plt.Rectangle(
                        (x-10, y-10), 20, 20, edgecolor='black', facecolor='yellow', fill=True, linewidth=2)
                    ax.add_patch(via)
                    count += 1

            prev_x = x
            prev_y = y
            prev_h = h
        line = fp.readline()
        line = fp.readline()
    # print(count)
    fp.close()

    plt.yticks(np.arange(0, arrange+100, 20))
    plt.xticks(np.arange(0, arrange+100, 20))
    plt.grid()
    plt.savefig(fig_filename)
    plt.show()
    print('[figure output]', fig_filename)


def fp_plot_by_RDL(arrange, FL, filename_pin, filename_bp_route, filename_2pin_nets_route, filename_3pin_net_res, fig_filename, RDLnum):

    fig = plt.figure(figsize=(20, 20))
    ax = plt.subplot(1, 1, 1)

    fp = open(filename_pin, "r")
    floorplane = plt.Rectangle(
        (0, 0), FL[0], FL[1], edgecolor='black', fill=False, linewidth=2)
    ax.add_patch(floorplane)
    chipdraw = plt.Rectangle((290, 473), 520, 474, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((215, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((615, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)

    offset_w = FL[0] - 160
    offset_h = FL[1] - 160
    offset_boundary = plt.Rectangle(
        (80, 80), offset_w, offset_h, edgecolor='black', fill=False, linewidth=2, linestyle='dashed',)
    txt = 'Offset_boundary ' + str(offset_w)+" x "+str(offset_h)
    plt.text(x=80, y=70, s=txt, fontsize=10, fontstyle='oblique')
    ax.add_patch(offset_boundary)

    c = 0
    line = fp.readline()
    while line != '':

        pos = line.split()
        x = float(pos[0])
        y = float(pos[1])
        w = 10.0
        h = 10.0
        name = "("+str(x)+",\n"+str(y) + ")"
        if (c >= 85+31+31):
            pad = plt.Rectangle((x-25, y-25), 50, 50, edgecolor='black',
                                facecolor='lightyellow', fill=True, linewidth=2)
            ax.add_patch(pad)
        c += 1
        # pin = plt.Rectangle((x-w/2, y-w/2), w, h, edgecolor='black',
        #                     facecolor='blue', fill=True, linewidth=2)
        # ax.add_patch(pin)
        #plt.text(x=x+int(w/2), y=y+int(h/2), s=name, fontsize=5, fontstyle='oblique')

        line = fp.readline()
    fp.close()

    # draw 3-pin net resource used
    fp = open(filename_3pin_net_res, "r")
    line = fp.readline()

    flag = False
    while line != '':
        pos = line.split()
        if(len(pos) < 5):
            flag = True
            netnum = "m" + pos[3]
            line = fp.readline()
            pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 0):
                x = float(pos[i][1:-1])
            elif(i % 4 == 1):
                y = float(pos[i][0:-1])
            elif(i % 4 == 2):
                h = float(pos[i][0:-1])
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1 and RDLnum == 1):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='red', alpha=0.8)
                elif (prev_h == 2 and h == 2 and RDLnum == 2):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='blue', alpha=0.8)
                elif (prev_h == 3 and h == 3 and RDLnum == 3):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='green', alpha=0.8)

                if (x == prev_x and y == prev_y and abs(h-prev_h) == 1):
                    color = ['lightyellow', 'pink', 'lightblue', 'lightgreen']
                    tile_used = plt.Rectangle(
                        (x-7, y-7), 14, 14, edgecolor='black', facecolor='lightyellow', fill=True, linewidth=1, alpha=0.8)
                    ax.add_patch(tile_used)
            if(i == 3 or (i == (len(pos)-1) and flag)):
                plt.text(x=x-4, y=y, s=netnum,
                         fontsize=10, fontstyle='oblique')
            prev_x = x
            prev_y = y
            prev_h = h
        flag = False
        line = fp.readline()
    fp.close()

    # draw 2-pin net routing paths...RDL1
    if RDLnum == 1:
        fp = open(filename_2pin_nets_route, "r")
        line = fp.readline()
        count = 0
        while line != '':
            pos = line.split()
            x, y, h = -1,  -1,  -1
            prev_x, prev_y, prev_h = -1,  -1, -1
            for i in range(len(pos)):
                if (i == 0):
                    pathname = pos[i][4:-1]
                if(i % 4 == 1):
                    x = float(pos[i][1:-1])
                    # print(x)
                elif(i % 4 == 2):
                    y = float(pos[i][0:-1])
                    # print(y)
                elif(i % 4 == 3):
                    h = float(pos[i][0:-1])
                    # print(h)
                if(i == 3):
                    plt.text(x=x-10, y=y, s=pathname,
                             fontsize=10, fontstyle='oblique')
                elif(i == len(pos)-1):
                    plt.text(x=x-10, y=y, s=pathname,
                             fontsize=10, fontstyle='oblique')
                if(prev_x != -1 and prev_y != -1):
                    if (prev_h == 1 and h == 1):
                        ax.plot([prev_x, x], [prev_y, y],
                                linewidth=10, color='red', alpha=0.8)
    #                 elif (prev_h == 2 and h ==2):
    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
    #                 elif (prev_h == 3 and h ==3):
    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
    #                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
                        #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
    #                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
    #                     ax.add_patch(via)
    #                     count+=1

                prev_x = x
                prev_y = y
                prev_h = h
            line = fp.readline()
            line = fp.readline()
        fp.close()

    # draw 2-pin net routing paths...RDL2
    if RDLnum == 2:
        fp = open(filename_2pin_nets_route, "r")
        line = fp.readline()
        count = 0
        while line != '':
            pos = line.split()
            x, y, h = -1,  -1,  -1
            prev_x, prev_y, prev_h = -1,  -1, -1
            for i in range(len(pos)):
                if(i % 4 == 1):
                    x = float(pos[i][1:-1])
                    # print(x)
                elif(i % 4 == 2):
                    y = float(pos[i][0:-1])
                    # print(y)
                elif(i % 4 == 3):
                    h = float(pos[i][0:-1])
                    # print(h)
                if(prev_x != -1 and prev_y != -1):
                    #                 if (prev_h == 1 and h ==1):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                    if (prev_h == 2 and h == 2):
                        ax.plot([prev_x, x], [prev_y, y],
                                linewidth=10, color='blue', alpha=0.8)
    #                 elif (prev_h == 3 and h ==3):
    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
    #                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
                        #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
    #                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
    #                     ax.add_patch(via)
    #                     count+=1

                prev_x = x
                prev_y = y
                prev_h = h
            line = fp.readline()
            line = fp.readline()
        fp.close()

    # draw 2-pin net routing paths...RDL3
    if RDLnum == 3:
        fp = open(filename_2pin_nets_route, "r")
        line = fp.readline()
        count = 0
        while line != '':
            pos = line.split()
            x, y, h = -1,  -1,  -1
            prev_x, prev_y, prev_h = -1,  -1, -1
            for i in range(len(pos)):
                if(i % 4 == 1):
                    x = float(pos[i][1:-1])
                    # print(x)
                elif(i % 4 == 2):
                    y = float(pos[i][0:-1])
                    # print(y)
                elif(i % 4 == 3):
                    h = float(pos[i][0:-1])
                    # print(h)
                if(prev_x != -1 and prev_y != -1):
                    #                 if (prev_h == 1 and h ==1):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                    #                 elif (prev_h == 2 and h ==2):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
                    if (prev_h == 3 and h == 3):
                        ax.plot([prev_x, x], [prev_y, y],
                                linewidth=10, color='green', alpha=0.8)
    #                 elif (prev_x==x and prev_y == y and prev_h!=-1): #via usage
    #                     #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
    #                     via = plt.Rectangle((x-5, y-5), 10, 10, facecolor='yellow', fill=True, alpha=1)
    #                     ax.add_patch(via)
    #                     count+=1

                prev_x = x
                prev_y = y
                prev_h = h
            line = fp.readline()
            line = fp.readline()
        fp.close()

    # draw 2-pin net routing paths...VIA
    fp = open(filename_2pin_nets_route, "r")
    line = fp.readline()
    count = 0
    while line != '':
        pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 1):
                x = float(pos[i][1:-1])
                # print(x)
            elif(i % 4 == 2):
                y = float(pos[i][0:-1])
                # print(y)
            elif(i % 4 == 3):
                h = float(pos[i][0:-1])
                # print(h)
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='pink', alpha=0.8)
                    a = 1
                elif (prev_h == 2 and h == 2):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightblue', alpha=0.8)
                    a = 1
                elif (prev_h == 3 and h == 3):
                    #                     ax.plot([prev_x, x],[prev_y, y], linewidth=10, color='lightgreen', alpha=0.8)
                    a = 1
                elif (prev_x == x and prev_y == y and prev_h != -1):  # via usage
                    #print(x," ", y," ", h, " - ",prev_x," ", prev_y," ", prev_h )
                    via = plt.Rectangle(
                        (x-10, y-10), 20, 20, edgecolor='black', facecolor='yellow', fill=True, linewidth=2)
                    ax.add_patch(via)
                    count += 1

            prev_x = x
            prev_y = y
            prev_h = h
        line = fp.readline()
        line = fp.readline()
    # print(count)
    fp.close()

    plt.yticks(np.arange(0, arrange+100, 20))
    plt.xticks(np.arange(0, arrange+100, 20))
    plt.grid()
    plt.savefig(fig_filename)
    plt.show()
    print('[figure output]', fig_filename)


def fp_plot_3pin(arrange, FL, filename_pin, filename_bp_route, filename_2pin_nets_route, filename_3pin_net_res):

    fig = plt.figure(figsize=(20, 20))
    ax = plt.subplot(1, 1, 1)

    fp = open(filename_pin, "r")
    floorplane = plt.Rectangle(
        (0, 0), FL[0], FL[1], edgecolor='black', fill=False, linewidth=2)
    ax.add_patch(floorplane)
    chipdraw = plt.Rectangle((290, 473), 520, 474, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((215, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)
    chipdraw = plt.Rectangle((615, 155), 270, 230, edgecolor='black',
                             facecolor='lightgrey', fill=True, linewidth=2)
    ax.add_patch(chipdraw)

    offset_w = FL[0] - 160
    offset_h = FL[1] - 160
    offset_boundary = plt.Rectangle(
        (80, 80), offset_w, offset_h, edgecolor='black', fill=False, linewidth=2, linestyle='dashed',)
    txt = 'Offset_boundary ' + str(offset_w)+" x "+str(offset_h)
    plt.text(x=80, y=70, s=txt, fontsize=10, fontstyle='oblique')
    ax.add_patch(offset_boundary)

    c = 0
    line = fp.readline()
    while line != '':

        pos = line.split()
        x = float(pos[0])
        y = float(pos[1])
        w = 10.0
        h = 10.0
        name = "("+str(x)+",\n"+str(y) + ")"
        if (c >= 90+33+33):
            pad = plt.Rectangle((x-25, y-25), 50, 50, edgecolor='black',
                                facecolor='lightyellow', fill=True, linewidth=2)
            ax.add_patch(pad)
        c += 1
        pin = plt.Rectangle((x-w/2, y-w/2), w, h, edgecolor='black',
                            facecolor='blue', fill=True, linewidth=2)
        ax.add_patch(pin)
        plt.text(x=x+int(w/2), y=y+int(h/2), s=name,
                 fontsize=5, fontstyle='oblique')

        line = fp.readline()
    fp.close()
    # draw transparent lines with width 10
    fp2 = open(filename_bp_route, "r")
    line2 = fp2.readline()
    while line2 != '':
        for i in range(3):
            line2 = fp2.readline()
            pos = line2.split()
            x1 = float(pos[0])
            y1 = float(pos[1])
            x2 = float(pos[3])
            y2 = float(pos[4])
            ax.plot([x1, x2], [y1, y2], linewidth=10,
                    color='lightgreen', alpha=0.8)
        line2 = fp2.readline()
    fp2.close()

    # draw black mid-lines with width 1
    fp2 = open(filename_bp_route, "r")
    line2 = fp2.readline()
    while line2 != '':
        for i in range(3):
            line2 = fp2.readline()
            pos = line2.split()
            x1 = float(pos[0])
            y1 = float(pos[1])
            x2 = float(pos[3])
            y2 = float(pos[4])
            ax.plot([x1, x2], [y1, y2], linewidth=1, color='black')
        line2 = fp2.readline()
    fp2.close()

    # draw 3-pin net resource used
    fp = open(filename_3pin_net_res, "r")
    line = fp.readline()

    flag = False
    while line != '':
        pos = line.split()
        if(len(pos) < 5):
            flag = True
            netnum = "$" + pos[3]
            line = fp.readline()
            pos = line.split()
        x, y, h = -1,  -1,  -1
        prev_x, prev_y, prev_h = -1,  -1, -1
        for i in range(len(pos)):
            if(i % 4 == 0):
                x = float(pos[i][1:-1])
            elif(i % 4 == 1):
                y = float(pos[i][0:-1])
            elif(i % 4 == 2):
                h = float(pos[i][0:-1])
            if(prev_x != -1 and prev_y != -1):
                if (prev_h == 1 and h == 1):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='pink', alpha=0.8)
                elif (prev_h == 2 and h == 2):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='lightblue', alpha=0.8)
                elif (prev_h == 3 and h == 3):
                    ax.plot([prev_x, x], [prev_y, y],
                            linewidth=10, color='lightgreen', alpha=0.8)

                if (x == prev_x and y == prev_y and abs(h-prev_h) == 1):
                    color = ['lightyellow', 'pink', 'lightblue', 'lightgreen']
                    tile_used = plt.Rectangle(
                        (x-7, y-7), 14, 14, edgecolor='black', facecolor='lightyellow', fill=True, linewidth=1, alpha=0.8)
                    ax.add_patch(tile_used)
            if(i == 3 or (i == (len(pos)-1) and flag)):
                plt.text(x=x-4, y=y, s=netnum,
                         fontsize=10, fontstyle='oblique')
            prev_x = x
            prev_y = y
            prev_h = h
        flag = False
        line = fp.readline()
    fp.close()

    plt.yticks(np.arange(0, arrange+100, 20))
    plt.xticks(np.arange(0, arrange+100, 20))
    plt.grid()
    plt.savefig('layout_3_pin_nets.png')
    plt.show()


if __name__ == '__main__':
    #filename_fig = sys.argv[1]
    filename_pinLocs = '../main/output/outpin.txt'
    # fp_plot(arrange=1100, FL=[1100, 1100],
    #         filename_pin='outpin.txt', filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets.txt',
    #         filename_3pin_net_res='maze_route_multipin.txt')
    # fp_plot_3pin(arrange=1100, FL=[1100, 1100],
    #              filename_pin='outpin.txt', filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets.txt',
    #              filename_3pin_net_res='maze_route_multipin.txt')

    fp_plot(arrange=1100, FL=[1100, 1100],
            filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_compact.txt',
            filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_compact.png')
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_compact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_compact_RDL1.png', RDLnum=1)
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_compact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_compact_RDL2.png', RDLnum=2)
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_compact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_compact_RDL3.png',  RDLnum=3)

    fp_plot(arrange=1100, FL=[1100, 1100],
            filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_uncompact.txt',
            filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_uncompact.png')
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_uncompact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_umcompact_RDL1.png', RDLnum=1)
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_uncompact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_umcompact_RDL2.png', RDLnum=2)
    fp_plot_by_RDL(arrange=1100, FL=[1100, 1100],
                   filename_pin=filename_pinLocs, filename_bp_route='bp_routing.txt', filename_2pin_nets_route='routing_path_2pin_nets_uncompact.txt',
                   filename_3pin_net_res='maze_route_multipin.txt', fig_filename='../output/layout_umcompact_RDL3.png',  RDLnum=3)
