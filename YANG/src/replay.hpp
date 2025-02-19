#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct ReplayStep{
	double leftWheels[4]; // wheels
    double rightWheels[4]; // wheels
	double intake; // intake
    double conveyor; // conveyor
    bool arm; // arm
    bool grabber; // goal  grabber
};

void write_replay(std::vector<ReplayStep> steps, std::string filename) {
	std::ofstream replay_file(std::string("/usd/") + filename);

	for(const auto step : steps) {
		replay_file << step.leftWheels[0] << " ";
		replay_file << step.leftWheels[1] << " ";
		replay_file << step.leftWheels[2] << " ";
		replay_file << step.leftWheels[3] << " ";

		replay_file << step.rightWheels[0] << " ";
		replay_file << step.rightWheels[1] << " ";
		replay_file << step.rightWheels[2] << " ";
		replay_file << step.rightWheels[3] << " ";

		replay_file << step.intake << " ";
		replay_file << step.conveyor << " ";
		replay_file << step.arm << " ";
		replay_file << step.grabber;
		replay_file << "\n";
	}
}

std::vector<ReplayStep> read_replay(std::string filename) {
	std::vector<ReplayStep> steps(0);

	std::ifstream replay_file(std::string("/usd/") + filename);
	std::string line;

	int i = 0;
	while (getline (replay_file, line, '\n')) {
		std::istringstream s(line);
		 while(!s.eof()) {
			ReplayStep current_step;
			s >> current_step.leftWheels[0];
			s >> current_step.leftWheels[1];
			s >> current_step.leftWheels[2];
			s >> current_step.leftWheels[3];
			s >> current_step.rightWheels[0];
			s >> current_step.rightWheels[1];
			s >> current_step.rightWheels[2];
			s >> current_step.rightWheels[3];
			s >> current_step.intake;
			s >> current_step.conveyor;
			s >> current_step.arm;
			s >> current_step.grabber;
			steps.push_back(current_step);
			if(s.fail()) {
				s.clear();
				std::string temp;
				s >> temp;
				continue;
			}
		}
		i++;
	}
	return steps;
}