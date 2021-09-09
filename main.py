# from os import mkdir
import cv2
import signal
from os import path, mkdir
# import path
import argparse
import numpy as np
from tqdm import tqdm
from time import sleep, time
from datetime import datetime
import matplotlib.pyplot as plt

TO_RELEASE = []
def sigint_handler(signum, frame):
    for x in TO_RELEASE:
        x.release()
    exit(0)


signal.signal(signal.SIGINT, sigint_handler)

def show_video(frames):
    for frame in frames:
        cv2.imshow('frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            exit(0)

def save_video(frames, name='default.mp4'):
    name = str(name)
    if name[:-4] != '.mp4':
        name += '.mp4'
    width, height = frames[0].shape[:2]
    out = cv2.VideoWriter(name, cv2.VideoWriter_fourcc(*'mp4v'), 24, (height, width), True)
    for frame in frames:
        out.write(frame)
    out.release()


def check_inputfile_argument(x):
    if x == 'output_<timestamp>.mp4':
        x = f"output_{datetime.utcfromtimestamp(time()).strftime('%Y-%m-%d_%H:%M:%S')}"
    if len(x) < 5 or x[-4:] != '.mp4':
        return f"output/{x}.mp4"
    return f"output/{x}"

def check_inputfile_existence(input):
    if path.isfile(input):
        return
    print(f"Could not find input file '{input}'")
    exit(0)

def make_output_dir():
    if path.exists('output') and not path.isfile('output'):
        return
    print(f"Making output directory")
    mkdir('output')
if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Max' art project",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('inputfile', help='Path to the input file')
    parser.add_argument('-o', '--output', metavar='FILENAME', 
                        default='output_<timestamp>.mp4', dest='outputfile', 
                        type=check_inputfile_argument,
                        help="output filename (will be stored in the directory\
                             output)")
    parser.add_argument('-s', '--show', action='store_true', default=False,
                        help='show each frame as it is constructed')
    parser.add_argument('-n', metavar='N', default='50', type=int,
                        help='number of frames to average')


    args = parser.parse_args()

    # print(args)
    # print('inputfile:', args.inputfile)
    # print('output:', args.outputfile)
    # print('n:', args.n)

    check_inputfile_existence(args.inputfile)
    make_output_dir()
    
    inp_first = cv2.VideoCapture(args.inputfile)
    inp_last = cv2.VideoCapture(args.inputfile)
    width, height = int(inp_first.get(cv2.CAP_PROP_FRAME_WIDTH)), int(inp_first.get(cv2.CAP_PROP_FRAME_HEIGHT))
    num_frames = int(inp_first.get(cv2.CAP_PROP_FRAME_COUNT))
    factor = 1 / args.n

    print(f"""\
width, height           : {width}, {height}
total number of frames  : {num_frames}
multiplication factor   : {factor}
using input file        : {args.inputfile}
saving video as         : {args.outputfile}
""")
    out = cv2.VideoWriter(args.outputfile, cv2.VideoWriter_fourcc(*'mp4v'), 24, (width, height), True)

    # Get first
    first_index = 0
    last_index = first_index + args.n
    # inp_last.set(cv2.CAP_PROP_POS_FRAMES, last_index)
    
    _, frame = inp_last.read()
    # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    frame = frame * factor
    TO_RELEASE.append(inp_first)
    TO_RELEASE.append(inp_last)
    TO_RELEASE.append(out)

    # Make average
    this_range = list(range(1, args.n))
    print(f"Making first average with frames '{this_range[0]}-{this_range[-1]}'")
    for i in tqdm(range(1, args.n), desc="Building first frame"):
        _, tmp = inp_last.read()
        # tmp = cv2.cvtColor(tmp, cv2.COLOR_BGR2GRAY)
        frame = cv2.add(frame, tmp * factor)

    this_range = list(range(args.n, num_frames))
    print(f"Making rest with frames '{this_range[0]}-{this_range[-1]}'")
    for i in tqdm(range(args.n, num_frames), desc="Building video"):
        # Subtract first
        # inp.set(cv2.CAP_PROP_POS_FRAMES, first_index)
        _, tmp = inp_first.read()
        # tmp =  cv2.cvtColor(tmp, cv2.COLOR_BGR2GRAY)
        frame = cv2.subtract(frame, tmp*factor)
        
        # Add new
        # inp.set(cv2.CAP_PROP_POS_FRAMES, last_index)
        _, tmp = inp_last.read()
        # tmp = cv2.cvtColor(tmp, cv2.COLOR_BGR2GRAY)
        frame = cv2.add(frame, tmp*factor)


        # Show
        # _, frame = inp.read()
        # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # first_index += 1
        # last_index += 1
        # if last_index > 300:
            # out.release()
            # exit(0)

        write_frame = frame.astype('uint8')
        out.write(write_frame)
        if args.show:
            cv2.imshow('frame', write_frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            inp_first.release()
            inp_last.release()
            out.release()
            exit(0)


    inp_first.release()
    inp_last.release()
    out.release()
    
    exit(0)
    
    
    # exit(0)

    cap = cv2.VideoCapture('movie.mp4')

    TOTAL_FRAMES = 1700
    N_FRAMES_AVG = 25
    TOTAL = TOTAL_FRAMES - N_FRAMES_AVG



    frames = np.array([cap.read()[1] for i in range(TOTAL_FRAMES)])
    # save_video(frames)
    # exit(0)
    # frames = np.array([cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) for frame in frames])
    # print(frames[0])
    print("Extracted frames")
    # print(frames.shape)
    # print(frames[0])

    # video = [x.mean(axis=0).astype('uint8') for x in [frames[i:i+N_FRAMES_AVG] for i in range(TOTAL_FRAMES - N_FRAMES_AVG)]]
    # print(video[0].shape)



    video = []
    for i in tqdm(range(TOTAL), desc="Loading"):
        avg = frames[i:i+N_FRAMES_AVG].mean(axis=0).astype('uint8')
        video.append(avg)
        # print(avg.shape)
        print("Made video")
        print(f"Max: {video[0].max()}, Min: {video[0].min()}")

        name = time()
        print(f"Saving to file with name {name}", name)
        save_video(video, name)

        input("Press key to show video ")
        show_video(video)





