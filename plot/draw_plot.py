import math
import gc
import sys
import ROOT
import numpy as npi
import copy
from array import array
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = 1;")
ROOT.TH1.AddDirectory(ROOT.kFALSE)
ROOT.gStyle.SetOptStat(0)
from ROOT import TColor
from ROOT import TGaxis
import gc
TGaxis.SetMaxDigits(2)
################################## MY SIGNAL AND SM BG ################################

def draw1dHist(A,textA="A", label_name="sample", can_name="can"):
    a,b = label_name.split("_")
    canvas = ROOT.TCanvas(can_name,can_name,10,10,1100,628)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0.05, 0.05, 1, 0.99 , 0)
    pad1.Draw()

    A.SetLineColor( 4 )
    A.SetLineWidth( 2 )
    A.SetTitle('(E(#pi^{#pm}) =' + b +')')
    A.GetXaxis().SetTitle(textA)
    A.GetYaxis().SetTitle('N_events')
    A.GetXaxis().SetTitleSize(0.05)
    A.GetYaxis().SetTitleSize(0.05)
    A.SetMaximum(1.2*A.GetMaximum())
    A.SetMinimum(0);
    A.GetYaxis().SetTitleOffset(0.7)
    A.Draw()
    canvas.Print("1D_" + b +"_"+ textA + ".png")
    del canvas
    gc.collect()

def drawCombine(H = [], HN=[]):
    canvas = ROOT.TCanvas("can_name","can_name",10,10,1100,628)
    canvas.cd()
    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0.05, 0.05, 1, 0.99 , 0)
    pad1.Draw()
    legend = ROOT.TLegend(0.3,0.6,0.4,0.87)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.03)
    can = 'can'
    for i in range(len(H)):
        a,b,c =HN[i].split("_")[0:3]
        can = c
        H[i].SetLineColor( i+1 )
        if i==0:
            H[i].SetTitle('')
            H[i].GetXaxis().SetTitle(c)
            H[i].GetYaxis().SetTitle('Fraction')
            H[i].GetXaxis().SetTitleSize(0.05)
            H[i].GetYaxis().SetTitleSize(0.05)
            H[i].SetMaximum(1.5*H[i].GetMaximum())
            H[i].SetMinimum(0);
            H[i].GetYaxis().SetTitleOffset(0.7)
            H[i].Draw()   
        H[i].Draw("Same")
        legend.AddEntry(H[i] ,b,'l')
    legend.Draw("same")
    canvas.Print("ND_" + can + ".png")
    del canvas
    gc.collect()

def compare2Hist(A, B, textA="A", textB="B", label_name="sample", can_name="can", text=""):

    a,b = label_name.split("_")
    canvas = ROOT.TCanvas(can_name,can_name,50,50,865,780)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0, 0.315, 1, 0.99 , 0)
    pad2=ROOT.TPad(pad_name, pad_name, 0, 0.0, 1, 0.305 , 0)
    pad1.Draw()
    pad2.Draw()
    pad2.SetGridy()
    pad2.SetTickx()
    pad1.SetBottomMargin(0.02)
    pad1.SetLeftMargin(0.14)
    pad1.SetRightMargin(0.05)
    pad2.SetTopMargin(0.1)
    pad2.SetBottomMargin(0.4)
    pad2.SetLeftMargin(0.14)
    pad2.SetRightMargin(0.05)
    pad2.SetFillStyle(0)
    pad1.SetFillStyle(0)
    pad1.cd()

    A.SetLineColor( 2 )
    B.SetLineColor( 4 )

    A.SetTitle('(E(#pi^{#pm}) =' + b +')      ' + can_name)
    A.GetYaxis().SetTitle('1/E dE/dx')
    A.GetXaxis().SetTitleSize(0.05)
    A.GetYaxis().SetTitleSize(0.05)
    A.GetXaxis().SetLabelSize(0)
    A.GetYaxis().SetTitleOffset(0.8)
    A.GetYaxis().SetTitleSize(0.07)
    A.GetYaxis().SetLabelSize(0.04)
    A.SetMaximum(1.2*max(A.GetMaximum(),B.GetMaximum()));
    A.SetMinimum(0.8*min(A.GetMinimum(),B.GetMinimum()));
    A.Draw()
    B.Draw('esame')

    legend = ROOT.TLegend(0.7,0.65,0.9,0.84)
    legend.AddEntry(A ,textA,'l')
    legend.AddEntry(B ,textB,'l')
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.Draw("same")
    label_cms=text
    Label_cms = ROOT.TLatex(0.17,0.85,label_cms)
    Label_cms.SetNDC()
    Label_cms.SetTextFont(61)
    Label_cms.Draw()


    pad2.cd()
    dummy_ratio = A.Clone()
    dummy_ratio.Divide(B)
    dummy_ratio.SetTitle("")
#    dummy_ratio.SetMarkerStyle(20)
#    dummy_ratio.SetMarkerSize(1.2)
#    dummy_ratio.GetXaxis().CenterTitle()
    dummy_ratio.GetYaxis().CenterTitle()
    dummy_ratio.GetXaxis().SetMoreLogLabels()
    dummy_ratio.GetXaxis().SetNoExponent()
    dummy_ratio.GetXaxis().SetTitleSize(0.04/0.3)
    dummy_ratio.GetYaxis().SetTitleSize(0.04/0.3)
    dummy_ratio.GetXaxis().SetTitleFont(42)
    dummy_ratio.GetYaxis().SetTitleFont(42)
    dummy_ratio.GetXaxis().SetTickLength(0.05)
    dummy_ratio.GetYaxis().SetTickLength(0.05)
    dummy_ratio.GetXaxis().SetLabelSize(0.115)
    dummy_ratio.GetYaxis().SetLabelSize(0.089)
    dummy_ratio.GetXaxis().SetLabelOffset(0.02)
    dummy_ratio.GetYaxis().SetLabelOffset(0.01)
    dummy_ratio.GetYaxis().SetTitleOffset(0.42)
    dummy_ratio.GetXaxis().SetTitleOffset(1.1)
    dummy_ratio.GetYaxis().SetNdivisions(504)
    dummy_ratio.GetYaxis().SetRangeUser(0,2)
#    dummy_ratio.SetStats(ROOT.kFALSE)
    dummy_ratio.GetYaxis().SetTitle('Ratio')
    if 'Alpha' in can_name:
        dummy_ratio.GetXaxis().SetTitle('#alpha')
    elif 'Beta' in can_name:
        dummy_ratio.GetXaxis().SetTitle('#beta')
    elif 'EP' in can_name and 'First' in can_name:
        dummy_ratio.GetXaxis().SetTitle('Radiation length')
    else:
        dummy_ratio.GetXaxis().SetTitle('Interaction length')

    
    dummy_ratio.Draw("e")
    dummy_ratio.Draw("AXISSAMEY+")
    dummy_ratio.Draw("AXISSAMEX+")


    canvas.Print("2H_" + b + "_" + can_name + ".png")
    del canvas
    gc.collect()

samples = [
#'RezaAnalysis_200GeV',
#'RezaAnalysis_100GeV',
#'RezaAnalysis_50GeV',
'RezaAnalysis_5gev',
'RezaAnalysis_10gev',
'RezaAnalysis_20gev',
'RezaAnalysis_100gev',
#'RezaAnalysis_5GeV',
#'RezaAnalysis_2GeV',
#'RezaAnalysis_1GeV'
'RezaAnalysis_50gev'
]

variable = ['fDp','fPi0','fPi0L', 'Class','fSP','SP', 'fHadECAL'
#,'HadEPFullsim_spHCAL'
#,'HadEPFullsim_spECAL_HCAL'
#,'HadEPFullsim_spECAL_ECAL'
#,'HadEPFullsim_spECAL'
#,'HadEPFullsim_spHCALFit'
#,'HadEPFullsim_spECALFit'
#,'HadEPFullsim_HCAL'
#,'FirstPi0EPFullsim'
#,'LatePi0EPFullsim'
#,'FirstPi0EPFit'
#,'LatePi0EPFit'
,'Alpha_HadEPFullsim_spHCAL'
,'Alpha_HadEPFullsim_spECAL_ECAL'
,'Alpha_HadEPFullsim_spECAL_HCAL'
,'Beta_HadEPFullsim_spHCAL'
,'Beta_HadEPFullsim_spECAL_ECAL'
,'Beta_HadEPFullsim_spECAL_HCAL'
,'AlphaBetaCorr_HadEPFullsim_spHCAL'
,'AlphaBetaCorr_HadEPFullsim_spECAL_ECAL'
,'AlphaBetaCorr_HadEPFullsim_spECAL_HCAL'
#,'HadEPFullsim_spECALFastsim'
]

for num, sample in enumerate(samples):
    for v in variable:
        print v
        file1 = ROOT.TFile.Open('../hists/Hist'+sample+'.root')
        if v not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue
        histA = file1.Get(v)
        print sample + v
        draw1dHist(histA, v, sample )
        del histA


Hist=[]
HistName=[]

for v in variable:
    for num, sample in enumerate(samples):
        file1 = ROOT.TFile.Open('../hists/Hist'+sample+'.root')
        if v not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue     
        Hist.append(file1.Get(v))
        HistName.append(sample+'_'+v)
#    drawCombine(Hist,HistName)
    del Hist[:] 
    del HistName[:]


Fitvariable = [
['HadEPFullsim_spHCAL', 'HadEPFullsim_spHCALFit'],
['HadEPFullsim_spECAL_ECAL', 'HadEPFullsim_spECAL_ECALFit'],
['HadEPFullsim_spECAL_HCAL', 'HadEPFullsim_spECAL_HCALFit'],
['FirstPi0EPFullsim_spHCAL','FirstPi0EPFullsim_spHCALFit'],
['FirstPi0EPFullsim_spECAL_HCAL','FirstPi0EPFullsim_spECAL_HCALFit'],
['FirstPi0EPFullsim_spECAL_ECAL','FirstPi0EPFullsim_spECAL_ECALFit'],
['LatePi0EPFullsim_spHCAL','LatePi0EPFullsim_spHCALFit'],
['LatePi0EPFullsim_spECAL_HCAL','LatePi0EPFullsim_spECAL_HCALFit'],
['LatePi0EPFullsim_spECAL_ECAL','LatePi0EPFullsim_spECAL_ECALFit']
]

for num, sample in enumerate(samples):
    for v in Fitvariable:
        file1 = ROOT.TFile.Open('../hists/Hist'+sample+'.root')
        if v[0] not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue
        histA = file1.Get(v[0])
        histB = file1.Get(v[1])
        compare2Hist(histA, histB,'FullSim', 'Fit', sample, v[0] )
        del histA
        del histB

#Fastsimvariable = [['HadEPFullsim_spHCAL', 'HadEPFastsim_spHCAL', 'Hadronic EP in HCAL for showers starting in HCAL'],
#['HadEPFullsim_spECAL_HCAL','HadEPFastsim_spECAL_HCAL' , 'Hadronic EP in HCAL for showers starting in ECAL'],
#['HadEPFullsim_spECAL_ECAL','HadEPFastsim_spECAL_ECAL' , 'Hadronic EP in ECAL for showers starting in ECAL'],
#['Beta_HadEPFullsim_spHCAL','Beta_HadEPFastsim_spHCAL' ,'Hadronic EP in HCAL for showers starting in HCAL'],
#['Beta_HadEPFullsim_spECAL_HCAL','Beta_HadEPFastsim_spECAL_HCAL', 'Hadronic EP in HCAL for showers starting in ECAL'],
#['Beta_HadEPFullsim_spECAL_ECAL','Beta_HadEPFastsim_spECAL_ECAL','Hadronic EP in ECAL for showers starting in ECAL'],
#['Alpha_HadEPFullsim_spHCAL','Alpha_HadEPFastsim_spHCAL', 'Hadronic EP in HCAL for showers starting in HCAL'],
#['Alpha_HadEPFullsim_spECAL_HCAL','Alpha_HadEPFastsim_spECAL_HCAL', 'Hadronic EP in HCAL for showers starting in ECAL'],
#['Alpha_HadEPFullsim_spECAL_ECAL','Alpha_HadEPFastsim_spECAL_ECAL', 'Hadronic EP in ECAL for showers starting in ECAL']
#]
#for num, sample in enumerate(samples):
#    for v in Fastsimvariable:
#        file1 = ROOT.TFile.Open('Hist'+sample+'.root')
#        if v[0] not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
#            print('do not find the hist')
#            continue
#        histA = file1.Get(v[0])
#        histB = file1.Get(v[1])
#        compare2Hist(histA, histB,'FullSim', 'Fastsim', sample, 'fastsim_'+v[0] , v[2])
#        del histA
#        del histB
#
#Othervariable = [['HadEPFullsim_spHCAL','HadEPFullsim_spECAL_HCAL', 'Hadronic EP in HCAL for all showers']]
#for num, sample in enumerate(samples):
#    for v in Othervariable:
#        file1 = ROOT.TFile.Open('Hist'+sample+'.root')
#        if v[0] not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
#            print('do not find the hist')
#            continue
#        histA = file1.Get(v[0])
#        histB = file1.Get(v[1])
#        compare2Hist(histA, histB,'SP HCAL', 'SPECAL', sample, 'SP_'+v[0] , v[2])
#        del histA
#        del histB
