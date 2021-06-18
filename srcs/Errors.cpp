#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_WASNOSUCHNICK 406
#define ERR_TOOMANYTARGETS 407
#define ERR_NOORIGIN 409
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413
#define ERR_WILDTOPLEVEL 414
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NOMOTD 422
#define ERR_NOADMININFO 423
#define ERR_FILEERROR 424
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOLOGIN 444
#define ERR_SUMMONDISABLED 445
#define ERR_USERSDISABLED 446
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_NOPERMFORHOST 463
#define ERR_PASSWDMISMATCH 464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_KEYSET 467
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_NOPRIVILEGES 481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_CANTKILLSERVER 483
#define ERR_NOOPERHOST 491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502

#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

void write_error(int code, Client *client, Server *serv, Channel *chan, std::string param)
{
	std::string errmess;
	
	if (code == 401)
	{
		errmess += param + " :No such nick/channel";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 402)
	{
		errmess += param + " :No such server";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 403)
	{
		errmess += param + " :No such channel";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 404)
	{
		errmess += param + " :Cannot send to channel";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 405)
	{
		errmess += param + " :You have joined too many channels";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
}