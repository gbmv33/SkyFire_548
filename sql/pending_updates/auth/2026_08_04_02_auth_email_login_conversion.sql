ALTER TABLE `account`
    ADD COLUMN IF NOT EXISTS `email_login_converted` tinyint unsigned NOT NULL DEFAULT 0 COMMENT '1 when the account self-converted to email login' AFTER `reg_mail`;

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(232, 'Command: account convert'),
(233, 'Command: account convert email')
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`);

INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(199, 232),
(199, 233),
(232, 233);
