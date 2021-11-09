from __future__ import print_function
import math
import gc
import sys
import ROOT
import numpy as np
import copy
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = 1;")
ROOT.TH1.AddDirectory(ROOT.kFALSE)
ROOT.gStyle.SetOptStat(0)
from array import array
from ROOT import TColor
from ROOT import TGaxis
from ROOT import THStack
from ROOT import TFile
from ROOT import TCanvas
from ROOT import TLegend
from ROOT import TH1F
from ROOT import TGraphErrors
import gc

########################## define a function that return plots given a TFile #################################

def GetPlots(filename):
    file0=ROOT.TFile.Open(filename)
    dir1 = file0.Get("DQMData")
    dir2 = dir1.Get("Run 1")
    dir3 = dir2.Get("JetMET")
    dir4 = dir3.Get("Run summary")
    plots=[]
    for type in dir4.GetListOfKeys():
        print (type.GetName())
        dirType= dir4.Get(type.GetName())
        for triggerKey in dirType.GetListOfKeys():
            triggerDir=dirType.Get(triggerKey.GetName())
            for plotKey in triggerDir.GetListOfKeys():
                print (plotKey.GetName())
                plotPointer=triggerDir.Get(plotKey.GetName())
                plot=plotPointer
                if(plot.GetName()=="efficiency"): 
                    for plotEfficiencyKey in plotPointer.GetListOfKeys():
                        plot=plotPointer.Get(plotEfficiencyKey.GetName())
                        plots=plots+[plot.Clone(triggerKey.GetName() + "_" + plot.GetName())]
                else:
                    plots=plots+[plot.Clone(triggerKey.GetName() + "_" + plot.GetName())]
    
    return plots

########################### read DQM plots #################################
DQMfileFull="FullsimDQM.root"
DQMfileFast="FastsimDQM.root"
labelFast = "FastSim"
labelFull = "FullSim"

folder="plots"
try:
    os.mkdir(folder)
except:
    print("folder " + folder + " already exist")

plotsFast=0
plotsFull=0
plotsFast = GetPlots(DQMfileFast)
plotsFull = GetPlots(DQMfileFull)

##### for kind of plots save a .png superimposing the Fast with the Full #####

ROOT.gROOT.SetBatch()
ROOT.gStyle.SetOptStat(0)
c1 = TCanvas("c1","",1280,720)
c1.SetGridx()
c1.SetGridy()
legend = TLegend(0.75,0.85,0.87,0.93);

first=True
for plotFast in plotsFast:
    for plotFull in plotsFull:
        if(plotFast.GetName()==plotFull.GetName()):
            plotFull.SetLineColor(25)
            plotFull.SetMarkerColor(25)
            plotFull.SetFillColor(25)
            plotFast.SetLineColor(1)
            plotFast.SetMarkerColor(1)
#           plotFast.SetFillColor(2)
#           plotFast.SetFillStyle(3002)
            
            plotFast.SetLineWidth(2)
            plotFull.SetLineWidth(2)
            if first:
                legend.AddEntry(plotFast,labelFast,"l");
                legend.AddEntry(plotFull,labelFull,"l");
            
            plotFull.SetMaximum(1.05*max(plotFull.GetMaximum(),plotFast.GetMaximum(),1))
            plotFull.Draw("Hist")
            plotFast.Draw("Esame")
            legend.Draw()
            c1.SaveAs(folder+"/"+plotFast.GetName()+".png")
            first=False
            
