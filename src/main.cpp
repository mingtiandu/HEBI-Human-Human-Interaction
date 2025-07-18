/**
 * A Project using HEBI Robotics to deploy the interactions between humans
 * A virtual spring is connecting two Robotics by taking into account the impedance control
 * For more information about HEBI Robtoics, go to http://docs.hebi.us/tools.html#cpp-api
 * For more information about HHI (Human-human Interactions) Project, go to https://github.com/mingtiandu/
 * HHI
 * Mingtian DU
 * May 2024
 */

#include "group_command.hpp"
#include "group_feedback.hpp"
#include "lookup.hpp"
using namespace hebi;

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

// Change the limitation base on your need
#define maxTime 10
#define maxVelocity 1
#define maxEffort 1

double ToEffort(double feedbackPos, double targetPos) {
    double stiffness = 0.01;
    return stiffness * (targetPos - feedbackPos);
}

int main(int argc, char const* argv[]) {
    // Create a Lookup object
    Lookup lookup;

    // Wait 2 seconds for the module list to populate, and then print out its contents
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::cout << std::endl;
    auto entry_list = lookup.getEntryList();
    for (auto entry : *entry_list)
        std::cout
        << "Name: " << entry.name_ << std::endl
        << "Family: " << entry.family_ << std::endl << std::endl;

    printf("Connecting to HEBI Successful\n");

    auto group = lookup.getGroupFromNames({ "X8-3" }, { "Robot1","Robot2" }); // Change it to the name of your series
    int num_joints = group->size();

    hebi::GroupFeedback group_feedback(num_joints);
    hebi::GroupCommand group_command_ref(num_joints);
    hebi::GroupCommand group_command_eff(num_joints);

    // Set the frequency of receiving feedback in unit of Hz, Hashrate or OS may affact the computation
    // C++ and Linux work the best and minorly downgrade the maximum frequency of 1000 Hz
    group->setFeedbackFrequencyHz(1000);

    // Calibrate the HEBI robots, set reference Effort as zero before starting the program
    group_command_ref[0].settings().actuator().referenceEffort().set(0);
    group_command_ref[1].settings().actuator().referenceEffort().set(0);
    group->sendCommand(group_command_ref);

    Eigen::VectorXd eff_cmd(num_joints);
    Eigen::VectorXd pos_fdk(num_joints);
    Eigen::VectorXd vel_fdk(num_joints);
    Eigen::VectorXd eff_fdk(num_joints);
 
    group->getNextFeedback(group_feedback);
    pos_fdk = group_feedback.getPosition();
    vel_fdk = group_feedback.getVelocity();
    eff_fdk = group_feedback.getEffort();

    auto tic = std::chrono::system_clock::now();
    auto toc = std::chrono::system_clock::now() - tic;
    while(toc.count() <= maxTime && vel_fdk[0]<maxVelocity && vel_fdk[1] < maxVelocity && eff_fdk[0] < maxEffort && eff_fdk[1] < maxEffort){
        
        group->getNextFeedback(group_feedback);
        pos_fdk = group_feedback.getPosition();
        vel_fdk = group_feedback.getVelocity();
        eff_fdk = group_feedback.getEffort();

        // Obtain the amount of command effort from position feedback
        double effort = ToEffort(pos_fdk[0], pos_fdk[1]);
        eff_cmd[0] = effort;
        eff_cmd[1] = -effort;

        group_command_eff.setEffort(eff_cmd);
        // group->sendCommand(group_command_eff);  // !!! Uncomment it only if you are very clear about the controller
        toc = std::chrono::system_clock::now() - tic;
    }
    return 0;
}