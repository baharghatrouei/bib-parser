#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include <cli11/cli11.h>

#include <bib-parser/core/parser.h>
#include <bib-parser/bibliography/sorter.h>

unsigned char const ErrorNone = 0;
unsigned char const ErrorNoSubcommand = 1;
unsigned char const ErrorParserException = 2;

int runCheckApp(std::string const &inputFilePath, bool const verbose);
int runConvertApp(std::string const &inputFilePath, std::string const &outputFilePath, std::string const &configFilePath, TUCSE::Sorter::Criteria const &sortCriteria, TUCSE::Parser::OutputType const &outputType, bool const verbose);

int main(int argc, char **argv)
{
	std::string inputFilePath{""};
	std::string configFilePath{""};
	std::string outputFilePath{""};

	TUCSE::Sorter::Criteria sortCriteria{TUCSE::Sorter::Criteria::NoSort};
	TUCSE::Parser::OutputType outputType{TUCSE::Parser::OutputType::HTML};

	bool verbose{false};

	CLI::App app{"Parse and modify some nice stuff"};
	app.require_subcommand(1);
	app.add_flag("-v,--verbose", verbose, "Output recurring information about the application state.");

	CLI::App *convertApp = app.add_subcommand("convert", "Convert a BibTeX file to other file formats.");
	CLI::App *checkApp = app.add_subcommand("check", "Check a BibTeX file for semantic errors.");

	convertApp->add_option<std::string, std::string>("-i,--input", inputFilePath, "Path to the input file containing BibTeX definitions.")->required();
	convertApp->add_option<std::string, std::string>("-o,--output", outputFilePath, "Path pointing to where the output file will be put.")->required();
	convertApp->add_option<std::string, std::string>("-c,--config", configFilePath, "Path to the configuration file containing translation rules for semantic tags.")->required();
	convertApp->add_option("-s,--sort", sortCriteria, "Can be used to sort the BibTeX entries.")->transform(CLI::CheckedTransformer(TUCSE::Sorter::argumentMap, CLI::ignore_case));

	checkApp->add_option<std::string, std::string>("-i,--input", inputFilePath, "Path to the input file containing BibTeX definitions.")->required();

	CLI11_PARSE(app, argc, argv); // Throws if invalid arguments supplied

	if (*checkApp)
	{
		return runCheckApp(inputFilePath, verbose);
	}
	else if (*convertApp)
	{
		return runConvertApp(inputFilePath, outputFilePath, configFilePath, sortCriteria, outputType, verbose);
	}

	return ErrorNoSubcommand;
}

int runCheckApp(std::string const &inputFilePath, bool const verbose)
{
	TUCSE::Parser parser{inputFilePath};
	parser.setVerbose(verbose);

	try
	{
		parser.parseInput();
	}
	catch (...)
	{
		return ErrorParserException;
	}

	return ErrorNone;
}

int runConvertApp(std::string const &inputFilePath, std::string const &outputFilePath, std::string const &configFilePath, TUCSE::Sorter::Criteria const &sortCriteria, TUCSE::Parser::OutputType const &outputType, bool const verbose)
{
	TUCSE::Parser parser{inputFilePath, configFilePath, outputFilePath};
	parser.setVerbose(verbose);

	try
	{
		parser.parseConfig();
		parser.parseInput();

		if (sortCriteria != TUCSE::Sorter::Criteria::NoSort)
		{
			parser.sort(sortCriteria);
		}

		parser.generateOutput(outputType);
	}
	catch (...)
	{
		return ErrorParserException;
	}

	return ErrorNone;
}