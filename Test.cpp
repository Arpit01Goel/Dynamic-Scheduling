#include <gtest/gtest.h>
#include "main.cpp"

class RoomTest: public ::testing::Test {
    protected:
        Room* room;
    
    void SetUp() override {
        room = new Room();
    }
    void TearDown() override {
        delete room;
    }
};

TEST_F(RoomTest, Booking) {
    EXPECT_EQ(room->book("20000000", "22000000"), 1);
    EXPECT_EQ(room->book("22000000", "00000100"), 1);
    EXPECT_EQ(room->book("20010100", "22590100"), 1);
    EXPECT_EQ(room->book("15000000", "17290000"), 1);
    EXPECT_EQ(room->book("00000300", "00000403"), 1);
    EXPECT_EQ(room->book("20000000", "00000100"), 0);
}

TEST_F(RoomTest, ShortBooking) {
    EXPECT_EQ(room->book("20000000", "22000000"), 1); //short booking

}
TEST_F(RoomTest, LongBooking) {
    EXPECT_EQ(room->book("00000000", "23492010"), 1); //very long book
}

TEST_F(RoomTest, Deleting1) {
    EXPECT_EQ(room->del("00000000", "12040111"), 0);
    room->book("17050102", "19170102");
    EXPECT_EQ(room->del("18000102", "19170102"), 1); //user can remove part of booking
    
    
}

TEST_F(RoomTest, Deleting2) {
    room->book("01031301", "01051301");
    room->book("01071301", "01091301");
    EXPECT_EQ(room->del("01031301", "01091301"), 0);
    room->book("01051301", "01071301");
    EXPECT_EQ(room->del("01031301", "01091301"), 1);
}

TEST_F(RoomTest, Suggest) {
    room->book("01031301", "01051301");
    room->book("01071301", "01091301");
    EXPECT_EQ(room->suggest("00000000", "23593011").size(), 3);
    room->book("01051301", "01071301");
    EXPECT_EQ(room->suggest("00000000", "23593011").size(), 2);
}

TEST_F(RoomTest, Listing) {
    EXPECT_EQ(room->listBooking("00000000", "23593011").size(), 0);
    room->book("01031301", "01051301");
    room->book("01071301", "01091301");
    EXPECT_EQ(room->listBooking("00000000", "23593011").size(), 2);
    room->book("01051301", "01071301");
    EXPECT_EQ(room->listBooking("00000000", "23593011").size(), 1);
}
TEST_F(RoomTest, Save) {
    room->book("20000000", "22000000");
    room->book("22000000", "00000100");
    room->book("20010100", "22590100");
    room->book("15000000", "17290000");
    room->book("00000300", "00000403");
    room->book("20000000", "00000100");
    EXPECT_EQ(room->save("dataOutput.txt"),true);
}
TEST_F(RoomTest, Load) {
    EXPECT_EQ(room->load("dataOutput.txt"),true);

}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}