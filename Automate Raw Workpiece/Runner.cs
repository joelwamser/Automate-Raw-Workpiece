using System;
using System.Collections.Generic;
using System.Text;
using STEPNCLib;
using System.Net;
using System.Net.Sockets;
using System.Configuration;
using System.IO;

namespace Automate_Raw_Workpiece
{
    class Runner
    {
        //outputs a workpiece step file
        //a rawpiece step file
        //and a new stepnc file
        static void Main(string[] args)
        {
            string inputStepNCFileName = args[0];
            string outputWorkpieceFileName = "workpiece_" + inputStepNCFileName;
            outputWorkpieceFileName = outputWorkpieceFileName.Replace(".238", "");
            string finalFileName = "rawpieceadded_" + inputStepNCFileName;
            AptStepMaker asm1 = new AptStepMaker();
            asm1.Open238(inputStepNCFileName);
            long ID = asm1.GetCurrentWorkpiece();
            asm1.ExportWorkpiece(ID, outputWorkpieceFileName);

            string rawMaterialName = BBoxer.RunMain(outputWorkpieceFileName);
            Console.WriteLine(rawMaterialName);
            Console.ReadLine();
            asm1.Rawpiece(rawMaterialName);
            asm1.SaveAsP21(finalFileName);
        }
    }
}
