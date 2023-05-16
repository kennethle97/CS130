#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test {
  protected:
    NginxConfigStatement statement;
    NginxConfigParser parser;
    NginxConfig out_config;
};


// tests a correct simple nginx config file: tests at least:
// 1. normal type tokens work correctly
// 2. enclosed start and end blocks with an end of statement token before the end block (doesn't have to be on same line) works correctly
// 3. tests that double quotes work correctly
TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

// testing nested blocks
// note to self: another possible error here, parser seems to not be able to handle nested blocks
TEST_F(NginxConfigParserTest, NestedBlockConfig) {
  bool success = parser.Parse("nested_block_config", &out_config);

  EXPECT_TRUE(success);
}

// tests a correct simple nginx config file but with comments added in
TEST_F(NginxConfigParserTest, CommentConfig) {
  bool success = parser.Parse("comment_config", &out_config);

  EXPECT_TRUE(success);
}

// tests an incorrect nginx config file that is missing the ending "}", or end block
// note to self on possible error: this actually passes because semicolon is considered end of statement and passes
// so the config file will have a floating start block with no end block
TEST_F(NginxConfigParserTest, MissingEndBlockConfig) {
  bool success = parser.Parse("missing_end_block_config", &out_config);

  EXPECT_FALSE(success);
}

// tests an incorrect nginx file that is missing the end of statement token, or ";"
TEST_F(NginxConfigParserTest, MissingEndOfStatementConfig) {
  bool success = parser.Parse("missing_end_statement_config", &out_config);

  EXPECT_FALSE(success);
}

// tests an incorrect nginx file where right before EOF is a normal token type
TEST_F(NginxConfigParserTest, NormalAtEOFConfig) {
  bool success = parser.Parse("normal_at_config", &out_config);

  EXPECT_FALSE(success);
}

// testing whether parser return false when there is no normal token before a start block token
TEST_F(NginxConfigParserTest, NoNormalBeforeStartConfig) {
  bool success = parser.Parse("no_normal_before_start_block_config", &out_config);

  EXPECT_FALSE(success);
}

// testing if quotes used correctly in the config file returns true from the parser
TEST_F(NginxConfigParserTest, CorrectQuoteConfig) {
  bool success = parser.Parse("correct_quote_config", &out_config);

  EXPECT_TRUE(success);
}

// testing if quotes used incorrectly in the config file raises an error and returns false
TEST_F(NginxConfigParserTest, IncorrectQuoteConfig) {
  bool success = parser.Parse("incorrect_quote_config", &out_config);

  EXPECT_FALSE(success);
}

// this file is used for quick testing that will not be in the final push
TEST_F(NginxConfigParserTest, MyConfig) {
  bool success = parser.Parse("my_config", &out_config);

  EXPECT_FALSE(success);
}

// notes for self: test for case "{}" and for case where "<EOF>" is right after START BLOCK, or basically that the config file is completely empty.
// these should be legal in the grammar given, but in the parser DO NOT WORK

// testing for empty config file returns True from parser
TEST_F(NginxConfigParserTest, EmptyConfig) {
  bool success = parser.Parse("empty_config", &out_config);

  EXPECT_TRUE(success);
}

// testing for empty directive block, i.e. "{}"
TEST_F(NginxConfigParserTest, EmptyBlockConfig) {
  bool success = parser.Parse("empty_block_config", &out_config);

  EXPECT_TRUE(success);
}

// testing for missing start block to a block directive
TEST_F(NginxConfigParserTest, MissingStartBlockConfig) {
  bool success = parser.Parse("missing_start_block_config", &out_config);

  EXPECT_FALSE(success);
}

// testing for port # directive in not nested block
TEST_F(NginxConfigParserTest, ListenPortNumber) {
  bool success = parser.Parse("listen_port_config", &out_config);
  int server_port = out_config.GetServerPort();

  EXPECT_TRUE(success);
  EXPECT_EQ(server_port, 80);
}

// testing for port # in a nested block
// TEST_F(NginxConfigParserTest, ListenPortNumberNested) {
//   bool success = parser.Parse("listen_port_nested_config", &out_config);
//   int server_port = out_config.GetServerPort();

//   EXPECT_TRUE(success);
//   EXPECT_EQ(server_port, 200);
// }

// testing for return -1 as port number if one was not seen in nginx config file
TEST_F(NginxConfigParserTest, NoListenPort) {
  bool success = parser.Parse("no_listen_port_config", &out_config);
  int server_port = out_config.GetServerPort();

  EXPECT_TRUE(success);
  EXPECT_EQ(server_port, -1);
}

// testing ToString method for NginxConfig 
TEST_F(NginxConfigParserTest, ToStringConfig) {
  bool success = parser.Parse("to_string_config", &out_config);
  std::string statement = out_config.ToString(1);
  std::string expected_statement = "  foo \"bar\";\n";
  EXPECT_TRUE(success);
  EXPECT_EQ(expected_statement, statement);
}

// testing if single quote is accounted for in NginxConfig correctly.
TEST_F(NginxConfigParserTest, SingleQuoteConfig) {
  bool success = parser.Parse("single_quote_config", &out_config);

  EXPECT_TRUE(success);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

