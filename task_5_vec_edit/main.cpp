// task_5_vec_edit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

int main()
{
    VecController controller;

    //Fill model with GUI
    auto pp1 = controller.CreatePrimitive<VecPoint>();
    auto pp2 = controller.CreatePrimitive<VecPoint>(5, 10);
    auto pl1 = controller.CreatePrimitive<VecLine>();
    auto pl2 = controller.CreatePrimitive<VecLine>(*pp1, *pp2);
    auto pr1 = controller.CreatePrimitive<VecRect>();

    //Show model on view
    std::cout << "Create model:" << std::endl;
    controller.UpdateScreenView();

    //Make some changes on model via GUI
    controller.Select(0);
    controller.Select(1);
    controller.MoveSelected(10, 20);
    controller.UnselectAll();
    controller.Select(2);
    controller.ResizeSelected(10, 10);
    controller.Unselect(2);
    std::cout << "Move and resize model:" << std::endl;
    controller.UpdateScreenView();

    controller.DeletePrimitive(2);
    std::cout << "Delete from model:" << std::endl;
    controller.UpdateScreenView();

    std::cout << "Save model to Temp.vec" << std::endl;
    controller.SaveModel("Temp.vec");
    std::cout << "Load model from Temp.vec" << std::endl;
    controller.LoadModel("Temp.vec");
    controller.UpdateScreenView();

    std::cout << "Move loaded model" << std::endl;

    controller.Select(0);
    controller.MoveSelected(10, 20);
    controller.UnselectAll();
    controller.UpdateScreenView();

    return 0;
}