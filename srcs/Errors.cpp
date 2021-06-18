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
		errmess += param + " :No such nick/channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 402)
	{
		errmess += param + " :No such server\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 403)
	{
		errmess += param + " :No such channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 404)
	{
		errmess += param + " :Cannot send to channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 405)
	{
		errmess += param + " :You have joined too many channels\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 406)
	{
		errmess += param + " :There was no such nickname\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 407)
	{
		errmess += param + " :Duplicate recipients. No message delivered\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code = 409)
	{
		errmess += ":No origin specified\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 411)
	{
		errmess += ":No recipient given " + param + "\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 412)
	{
		errmess += ":No text to send\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 413)
	{
		errmess += param + " :No toplevel domain specified\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 414)
	{
		errmess += param + " :Wildcard in toplevel domain\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 421)
	{
		errmess += param + " :Unknown command\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 422)
	{
		errmess += ":MOTD File is missing\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 423)
	{
		errmess += param + " :No administrative info available";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 424)
	{
		errmess += "Cheesh\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 431)
	{
		errmess += ":No nickname given\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 432)
	{
		errmess += " :Erroneus nickname\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 433)
	{
		errmess += param + " :Nickname is already in use\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 436)
	{
		errmess += param + " :Nickname collision KILL\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 441)
	{
		errmess += param + " :They aren't on that channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 442)
	{
		errmess += param + " :You're not on that channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 443)
	{
		errmess += param + " :is already on channel\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 444)
	{
		errmess += param + " :User not logged in\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 445)
	{
		errmess += ":SUMMON has been disabled\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 446)
	{
		errmess += ":USERS has been disabled\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 451)
	{
		errmess += ":You have not registered\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 461)
	{
		errmess += param + " :Not enough parameters\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 462)
	{
		errmess += ":You may not reregister\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 463)
	{
		errmess += ":Your host isn't among the privileged\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 464)
	{
		errmess += ":Password incorrect\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 465)
	{
		errmess += ":You are banned from this server\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 467)
	{
		errmess += param + " :Channel key already set\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 471)
	{
		errmess += param + " :Cannot join channel (+l)\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 472)
	{
		errmess += param + " :is unknown mode char to me\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 473)
	{
		errmess += param + " :Cannot join channel (+i)\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 474)
	{
		errmess += param + " :Cannot join channel (+b)\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 475)
	{
		errmess += param + " :Cannot join channel (+k)\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 481)
	{
		errmess += ":Permission Denied- You're not an IRC operator\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 482)
	{
		errmess += param + " :You're not channel operator\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 483)
	{
		errmess += ":You cant kill a server!\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 491)
	{
		errmess += ":No O-lines for your host\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 501)
	{
		errmess += ":Unknown MODE flag\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
	if (code == 502)
	{
		errmess += ":Cant change mode for other users\n";
		serv->fdwrite(client->getFd(), errmess);
		return ;
	}
}