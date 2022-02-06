DELETE FROM `command` WHERE name IN ('chat');

INSERT INTO `command` (`name`, `security`, `help`) VALUES 
('chat', 0, 'Syntax: .chat $text\n.chat on To show Global Chat\n.chat off To hide Global Chat');
