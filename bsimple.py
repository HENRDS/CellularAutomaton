#!/usr/bin/python3

import tkinter as tk
from sympy import *

class MainWindow(tk.Tk):
    def __init__(self, vrs, res, ready, master=None):
        tk.Tk.__init__(self, master)
        self.ready = ready
        self.bvars = vrs
        self.res = res#{ name : [] for name in res}
        self.canvas = tk.Canvas(self)
        self.btn = tk.Button(self.canvas, command=self.process, text='process')
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
        fmt = ' '.join([f'{{args[{i}]:{len(v)}}}' for i, v in  enumerate(self.bvars)])
        htxt = fmt.format(args=self.bvars)
        h = tk.Label(self.main_panel, text=htxt)
        h.grid(column=0, columnspan=self.nvars, row=0,sticky=tk.NSEW)
        for i in range(0, 2 ** self.nvars):
            l = tk.Label(self.main_panel, text=fmt.format(
                args=list(format(i, f'0{self.nvars}b'))))
            l.grid(column=0, columnspan=self.nvars, row=i+1, sticky=tk.NSEW)
        self.lhs = h

    def draw(self):
        self.nvars = len(self.bvars)
        self.main_panel = tk.Frame(self.canvas)
        # var headers
        # h = tk.Label(self.main_panel, text=' '.join(self.bvars))
        # h.grid(column=0, columnspan=self.nvars, row=0, sticky=tk.NSEW)
        # for i, v in enumerate(self.bvars):
        #     t = tk.Label(self.main_panel, text=v)
        #     t.grid(column=i, columnspan=1, row=0, sticky=tk.NSEW)
        tk.Label(self.main_panel, text=' |=| ').grid(
            column=self.nvars, columnspan=1, row=0, sticky=tk.NSEW)
        # Res headers
        for i, v in enumerate(self.res.keys()):
            t = tk.Label(self.main_panel, text=v)
            t.grid(column=i + self.nvars + 1, columnspan=1, row=0, sticky=tk.NSEW)
        self.build_lhs()
        for i in range(0, 2 ** self.nvars):
            # c = tk.Label(self.main_panel, text=' '.join(list(format(i, f?'0{self.nvars}b'))))
            # c.grid(column=0, columnspan=self.nvars, row=i+1, sticky=tk.NSEW)
            # for n , v in enumerate(self.bvars):
            #     v = self.true_im if (i & (1 << (self.nvars - (n+1)))) > 0 else self.false_im
            #     c = tk.Label(self.main_panel, image=v)
            #     c.grid(column=n, columnspan=1, row=i+1, sticky=tk.NSEW)
            for n, r in enumerate(self.res.keys()):
                vv = tk.BooleanVar(self, value=False)
                c = tk.Checkbutton(self.main_panel, variable=vv)
                self.res[r].append(vv)
                c.grid(column=self.nvars+1+n, row=i+1, sticky=tk.NSEW)
        sbar = tk.Scrollbar(self, command=self.canvas.yview)
        self.canvas.scrollbar = sbar
        sbar.grid(column=1, sticky=tk.NSEW)
        self.main_panel.grid(column=0, row=1, sticky=tk.NSEW)
        self.canvas.grid(column=0, row=0, sticky=tk.NSEW)
        self.canvas.config(yscrollcommand=sbar.set)

    def on_close(self):
        data = { k : [b.get() for b in v] for k,v in self.res.items()}
        with open('last', 'w') as f:
            f.write(str(data))
        self.destroy()

v = ['n', 's', 'e', 'w', 'nw', 'ne', 'sw', 'se'] 
# w = MainWindow(['a', 'b'], ['r'])
w = MainWindow(v,{x : [] for x in v}, False)
w.mainloop()
