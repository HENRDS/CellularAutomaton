#!/usr/bin/python3
import os
import tkinter as tk
import tkinter.tix as tix
from sympy import *


class MainWindow(tk.Tk):
    def __init__(self, vrs, res, ready, master=None):
        tk.Tk.__init__(self, master)
        # self.geometry("800x800")
        self.ready = ready
        self.bvars = vrs
        # self.resizable(width=False, height=False)
        self.main_panel = tk.Frame(self)
        self.main_panel.grid(row=1, sticky=tk.NSEW)
        self.canvas = tk.Canvas(self.main_panel)
        self.vsbar = tk.Scrollbar(
            self.canvas, orient="vertical", command=self.canvas.yview)
        self.vsbar.grid(row=0, column=1, sticky=tk.NS)
        self.canvas.configure(yscrollcommand=self.vsbar.set)
        if ready:
            self.res = {}
            for k, v in res.items():
                self.res[k] = [tk.BooleanVar(master=self.main_panel, value=b) for b in v]
        else:
            self.res = res
        self.btn = tk.Button(self, command=self.process, text='process')
        self.btn.grid(sticky=tk.NSEW, column=0, row=0)
        self.draw()
        self.grid()
        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def process(self):
        syms = list(symbols(','.join(self.bvars)))
        for r, vs in self.res.items():
            prods = []
            for i, bv in enumerate(vs):
                b = bv.get()
                if b:
                    prods.append([int(s) for s in list(format(i, f'0{self.nvars}b'))])
            print(f'{r}:')
            print(SOPform(syms, prods))

    def build_lhs(self):
        fmt = ' '.join([f'{{args[{i}]:{len(v)}}}' for i, v in enumerate(self.bvars)])
        htxt = fmt.format(args=self.bvars)

        h = tk.Label(self.main_panel, text=htxt)
        h.grid(column=0, columnspan=self.nvars, row=0, sticky=tk.NSEW)
        # lb = tk.Listbox(self.main_panel)
        for i in range(0, 2 ** self.nvars):
            txt = fmt.format(args=list(format(i, f'0{self.nvars}b')))
            print(txt)
            l = tk.Label(self.main_panel, text=txt)
            # lb.insert(tk.END, txt)
            l.grid(column=0, columnspan=self.nvars, row=i + 1, sticky=tk.NSEW)
        # lb.grid(column=0, row=1, sticky=tk.NSEW)
        self.lhs = h

    def draw(self):
        self.nvars = len(self.bvars)
        tk.Label(self.main_panel, text=' |=| ').grid(
            column=self.nvars, columnspan=1, row=0, sticky=tk.NSEW)
        # Res headers
        for i, v in enumerate(self.res.keys()):
            t = tk.Label(self.main_panel, text=v)
            t.grid(column=i + self.nvars + 1, columnspan=1, row=0, sticky=tk.NSEW)
        self.build_lhs()
        for i in range(0, 2 ** self.nvars):
            self.refs = []
            for n, r in enumerate(self.res.keys()):
                c = tk.Checkbutton(self.main_panel, variable=self.res[r][i] if self.ready else tk.BooleanVar(value=
                                                                                                           False))
                c.grid(column=self.nvars + 1 + n, row=i + 1, sticky=tk.NSEW)
                self.refs.append(c)
        # sbar = tk.Scrollbar(self, command=self.yview)
        # self.scrollbar = sbar
        # sbar.grid(column=1, sticky=tk.NSEW)
        
        # self.grid(column=0, row=0, sticky=tk.NSEW)
        # self.config(yscrollcommand=sbar.set)

    def on_close(self):
        data = {k: [b.get() for b in v] for k, v in self.res.items()}
        with open('last', 'w') as f:
            f.write(str(data))
        self.destroy()


v = ['n', 's', 'e', 'w', 'nw', 'ne', 'sw', 'se']
# res = {x: [] for x in v}
res = {'n': [] }
r = False
# if os.path.isfile('last'):
#     print('found')
#     res = eval(open('last', 'r').read())
#     r = True
w = MainWindow(v, res, r)
w.mainloop()
