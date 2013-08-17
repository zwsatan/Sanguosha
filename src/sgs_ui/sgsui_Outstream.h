#ifndef SGSUI_OUTSTREAM_H_
#define SGSUI_OUTSTREAM_H_

namespace sgs {
namespace DataType {
class Message;
}
}

namespace sgsui {

class Outstream
{
public:
	// create players in window, call this method when all players have been constructed
	Outstream& operator<<(int);

	Outstream& operator<<(const sgs::DataType::Message *);
};

} /* namespace sgsui */

#endif /* SGSUI_OUTSTREAM_H_ */
