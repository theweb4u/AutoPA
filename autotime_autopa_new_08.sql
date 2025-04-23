-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Apr 21, 2025 at 09:51 AM
-- Server version: 5.7.23-23
-- PHP Version: 8.1.32

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `autotime_autopa`
--

-- --------------------------------------------------------

--
-- Table structure for table `breaks`
--

CREATE TABLE `breaks` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `type` enum('Morning Tea','Lunch','Afternoon Tea') COLLATE utf8_unicode_ci DEFAULT NULL,
  `start_time` time DEFAULT NULL,
  `end_time` time DEFAULT NULL,
  `variance` int(11) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `breaks`
--

INSERT INTO `breaks` (`id`, `user_id`, `type`, `start_time`, `end_time`, `variance`) VALUES
(50, 1, 'Morning Tea', '11:00:00', '11:15:00', 0),
(51, 1, 'Lunch', '12:00:00', '13:00:00', 0),
(52, 1, 'Afternoon Tea', '15:00:00', '15:15:00', 0),
(53, 6, 'Morning Tea', '10:05:00', '10:15:00', 10),
(54, 6, 'Lunch', '12:30:00', '13:30:00', 30),
(55, 6, 'Afternoon Tea', '15:00:00', '15:15:00', 10);

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `title` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `description` text COLLATE utf8_unicode_ci,
  `parent_id` int(11) DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`id`, `user_id`, `title`, `description`, `parent_id`, `created_at`) VALUES
(1, 1, 'Work', 'New Description for Work', NULL, '2025-04-15 03:58:03'),
(2, 1, 'Personal', 'Personal projects', NULL, '2025-04-15 03:58:03'),
(5, 1, 'Meetings', 'Work meetings', 1, '2025-04-15 04:06:20'),
(6, 3, 'Home', 'Household tasks', NULL, '2025-04-15 04:07:00'),
(7, 1, 'General', NULL, NULL, '2025-04-18 01:09:09'),
(8, 1, 'Ben and Ems', 'Stuff to do at Ben and Ems in Helensville', NULL, '2025-04-18 20:42:35'),
(9, 6, 'Mangatoe', 'Work at Mangatoetoe Street house', NULL, '2025-04-18 23:18:48'),
(10, 6, '3 Waitete', 'Stuff to do at 3 Waitet Road property', NULL, '2025-04-18 23:20:49'),
(11, 6, '5 Waitete', 'Stuff to do at 5 Waitete property', NULL, '2025-04-18 23:21:57'),
(12, 6, 'Ben and Ems Helensville', 'Stuff to do at Bens in Helensville', NULL, '2025-04-18 23:23:16'),
(13, 6, 'Computer work', 'Work I need to do on the computer', NULL, '2025-04-20 20:18:52'),
(14, 6, 'Personal', 'All personal Tasks', NULL, '2025-04-20 20:36:42'),
(15, 6, 'Volantary', 'All voluntary work', NULL, '2025-04-20 21:25:37'),
(16, 1, 'Meeting Prep', 'All meetings', 1, '2025-04-20 22:48:16'),
(17, 1, 'Writing Latters', 'When I write Letters', 2, '2025-04-21 00:51:16'),
(18, 1, 'Household', 'Do household stuff', NULL, '2025-04-21 00:58:24'),
(19, 1, 'Chores', 'Do chores', 18, '2025-04-21 00:59:11'),
(20, 6, 'Will', 'Get my Will done and signed off', 13, '2025-04-21 02:32:51'),
(21, 6, 'Paeroa', 'Anything to do in Paeroa', NULL, '2025-04-21 03:26:28'),
(22, 6, 'Going To', 'Places I am going to so need to do stuff there', NULL, '2025-04-21 03:27:15');

-- --------------------------------------------------------

--
-- Table structure for table `events`
--

CREATE TABLE `events` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `title` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `details` text COLLATE utf8_unicode_ci,
  `date` date DEFAULT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `location` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `events`
--

INSERT INTO `events` (`id`, `user_id`, `title`, `details`, `date`, `start_time`, `end_time`, `location`, `created_at`) VALUES
(31, 1, 'Do AutoPA', 'Just to test it', '2025-04-22', '11:14:00', '12:14:00', 'Home', '2025-04-20 23:15:06');

-- --------------------------------------------------------

--
-- Table structure for table `preferences`
--

CREATE TABLE `preferences` (
  `user_id` int(11) NOT NULL,
  `time_zone` varchar(50) COLLATE utf8_unicode_ci DEFAULT 'UTC'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `preferences`
--

INSERT INTO `preferences` (`user_id`, `time_zone`) VALUES
(1, 'Pacific/Auckland'),
(6, 'Pacific/Auckland');

-- --------------------------------------------------------

--
-- Table structure for table `schedules`
--

CREATE TABLE `schedules` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `task_id` int(11) DEFAULT NULL,
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `date` date NOT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `type` enum('task','break','event') COLLATE utf8_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `schedules`
--

INSERT INTO `schedules` (`id`, `user_id`, `task_id`, `title`, `date`, `start_time`, `end_time`, `type`) VALUES
(1008, 1, NULL, 'Morning Tea', '2025-04-21', '11:00:00', '11:15:00', 'break'),
(1009, 1, NULL, 'Lunch', '2025-04-21', '12:00:00', '13:00:00', 'break'),
(1010, 1, NULL, 'Afternoon Tea', '2025-04-21', '15:00:00', '15:15:00', 'break'),
(1011, 1, 93, 'Meeting Prep', '2025-04-21', '13:19:00', '13:34:00', 'task'),
(1012, 1, 94, 'Test AutoPA', '2025-04-21', '13:49:00', '14:14:00', 'task'),
(1013, 1, 102, 'Install Roof', '2025-04-21', '14:19:00', '14:49:00', 'task'),
(1014, 1, 102, 'Install Roof (Part)', '2025-04-21', '15:19:00', '16:19:00', 'task'),
(1015, 1, NULL, 'Morning Tea', '2025-04-22', '11:00:00', '11:15:00', 'break'),
(1016, 1, NULL, 'Lunch', '2025-04-22', '12:00:00', '13:00:00', 'break'),
(1017, 1, NULL, 'Afternoon Tea', '2025-04-22', '15:00:00', '15:15:00', 'break'),
(1018, 1, NULL, 'Do AutoPA', '2025-04-22', '11:14:00', '12:14:00', 'event'),
(1019, 1, 102, 'Install Roof', '2025-04-22', '09:00:00', '11:00:00', 'task'),
(1020, 1, 102, 'Install Roof (Part)', '2025-04-22', '13:00:00', '14:45:00', 'task'),
(1021, 1, 121, 'Team Sync', '2025-04-22', '15:30:00', '16:30:00', 'task'),
(1022, 1, NULL, 'Morning Tea', '2025-04-23', '11:00:00', '11:15:00', 'break'),
(1023, 1, NULL, 'Lunch', '2025-04-23', '12:00:00', '13:00:00', 'break'),
(1024, 1, NULL, 'Afternoon Tea', '2025-04-23', '15:00:00', '15:15:00', 'break'),
(1025, 1, 98, 'Paint Bedroom', '2025-04-23', '09:00:00', '11:00:00', 'task'),
(1026, 1, 96, 'Do Ironing', '2025-04-23', '11:30:00', '12:00:00', 'task'),
(1027, 1, 96, 'Do Ironing (Part)', '2025-04-23', '13:00:00', '13:15:00', 'task'),
(1028, 1, NULL, 'Morning Tea', '2025-04-24', '11:00:00', '11:15:00', 'break'),
(1029, 1, NULL, 'Lunch', '2025-04-24', '12:00:00', '13:00:00', 'break'),
(1030, 1, NULL, 'Afternoon Tea', '2025-04-24', '15:00:00', '15:15:00', 'break'),
(1031, 1, NULL, 'Morning Tea', '2025-04-25', '11:00:00', '11:15:00', 'break'),
(1032, 1, NULL, 'Lunch', '2025-04-25', '12:00:00', '13:00:00', 'break'),
(1033, 1, NULL, 'Afternoon Tea', '2025-04-25', '15:00:00', '15:15:00', 'break'),
(1034, 1, NULL, 'Morning Tea', '2025-04-26', '11:00:00', '11:15:00', 'break'),
(1035, 1, NULL, 'Lunch', '2025-04-26', '12:00:00', '13:00:00', 'break'),
(1036, 1, NULL, 'Afternoon Tea', '2025-04-26', '15:00:00', '15:15:00', 'break'),
(1037, 1, NULL, 'Morning Tea', '2025-04-28', '11:00:00', '11:15:00', 'break'),
(1038, 1, NULL, 'Lunch', '2025-04-28', '12:00:00', '13:00:00', 'break'),
(1039, 1, NULL, 'Afternoon Tea', '2025-04-28', '15:00:00', '15:15:00', 'break'),
(1040, 1, NULL, 'Morning Tea', '2025-04-29', '11:00:00', '11:15:00', 'break'),
(1041, 1, NULL, 'Lunch', '2025-04-29', '12:00:00', '13:00:00', 'break'),
(1042, 1, NULL, 'Afternoon Tea', '2025-04-29', '15:00:00', '15:15:00', 'break'),
(1043, 1, NULL, 'Morning Tea', '2025-04-30', '11:00:00', '11:15:00', 'break'),
(1044, 1, NULL, 'Lunch', '2025-04-30', '12:00:00', '13:00:00', 'break'),
(1045, 1, NULL, 'Afternoon Tea', '2025-04-30', '15:00:00', '15:15:00', 'break'),
(1046, 1, NULL, 'Morning Tea', '2025-05-01', '11:00:00', '11:15:00', 'break'),
(1047, 1, NULL, 'Lunch', '2025-05-01', '12:00:00', '13:00:00', 'break'),
(1048, 1, NULL, 'Afternoon Tea', '2025-05-01', '15:00:00', '15:15:00', 'break'),
(1049, 1, NULL, 'Morning Tea', '2025-05-02', '11:00:00', '11:15:00', 'break'),
(1050, 1, NULL, 'Lunch', '2025-05-02', '12:00:00', '13:00:00', 'break'),
(1051, 1, NULL, 'Afternoon Tea', '2025-05-02', '15:00:00', '15:15:00', 'break'),
(1052, 1, NULL, 'Morning Tea', '2025-05-03', '11:00:00', '11:15:00', 'break'),
(1053, 1, NULL, 'Lunch', '2025-05-03', '12:00:00', '13:00:00', 'break'),
(1054, 1, NULL, 'Afternoon Tea', '2025-05-03', '15:00:00', '15:15:00', 'break'),
(1055, 1, NULL, 'Morning Tea', '2025-05-05', '11:00:00', '11:15:00', 'break'),
(1056, 1, NULL, 'Lunch', '2025-05-05', '12:00:00', '13:00:00', 'break'),
(1057, 1, NULL, 'Afternoon Tea', '2025-05-05', '15:00:00', '15:15:00', 'break'),
(1058, 1, NULL, 'Morning Tea', '2025-05-06', '11:00:00', '11:15:00', 'break'),
(1059, 1, NULL, 'Lunch', '2025-05-06', '12:00:00', '13:00:00', 'break'),
(1060, 1, NULL, 'Afternoon Tea', '2025-05-06', '15:00:00', '15:15:00', 'break'),
(1061, 1, NULL, 'Morning Tea', '2025-05-07', '11:00:00', '11:15:00', 'break'),
(1062, 1, NULL, 'Lunch', '2025-05-07', '12:00:00', '13:00:00', 'break'),
(1063, 1, NULL, 'Afternoon Tea', '2025-05-07', '15:00:00', '15:15:00', 'break'),
(1064, 1, NULL, 'Morning Tea', '2025-05-08', '11:00:00', '11:15:00', 'break'),
(1065, 1, NULL, 'Lunch', '2025-05-08', '12:00:00', '13:00:00', 'break'),
(1066, 1, NULL, 'Afternoon Tea', '2025-05-08', '15:00:00', '15:15:00', 'break'),
(1067, 1, NULL, 'Morning Tea', '2025-05-09', '11:00:00', '11:15:00', 'break'),
(1068, 1, NULL, 'Lunch', '2025-05-09', '12:00:00', '13:00:00', 'break'),
(1069, 1, NULL, 'Afternoon Tea', '2025-05-09', '15:00:00', '15:15:00', 'break'),
(1070, 1, NULL, 'Morning Tea', '2025-05-10', '11:00:00', '11:15:00', 'break'),
(1071, 1, NULL, 'Lunch', '2025-05-10', '12:00:00', '13:00:00', 'break'),
(1072, 1, NULL, 'Afternoon Tea', '2025-05-10', '15:00:00', '15:15:00', 'break'),
(1073, 1, NULL, 'Morning Tea', '2025-05-12', '11:00:00', '11:15:00', 'break'),
(1074, 1, NULL, 'Lunch', '2025-05-12', '12:00:00', '13:00:00', 'break'),
(1075, 1, NULL, 'Afternoon Tea', '2025-05-12', '15:00:00', '15:15:00', 'break'),
(1076, 1, NULL, 'Morning Tea', '2025-05-13', '11:00:00', '11:15:00', 'break'),
(1077, 1, NULL, 'Lunch', '2025-05-13', '12:00:00', '13:00:00', 'break'),
(1078, 1, NULL, 'Afternoon Tea', '2025-05-13', '15:00:00', '15:15:00', 'break'),
(1079, 1, NULL, 'Morning Tea', '2025-05-14', '11:00:00', '11:15:00', 'break'),
(1080, 1, NULL, 'Lunch', '2025-05-14', '12:00:00', '13:00:00', 'break'),
(1081, 1, NULL, 'Afternoon Tea', '2025-05-14', '15:00:00', '15:15:00', 'break'),
(1082, 1, NULL, 'Morning Tea', '2025-05-15', '11:00:00', '11:15:00', 'break'),
(1083, 1, NULL, 'Lunch', '2025-05-15', '12:00:00', '13:00:00', 'break'),
(1084, 1, NULL, 'Afternoon Tea', '2025-05-15', '15:00:00', '15:15:00', 'break'),
(1085, 1, NULL, 'Morning Tea', '2025-05-16', '11:00:00', '11:15:00', 'break'),
(1086, 1, NULL, 'Lunch', '2025-05-16', '12:00:00', '13:00:00', 'break'),
(1087, 1, NULL, 'Afternoon Tea', '2025-05-16', '15:00:00', '15:15:00', 'break'),
(1088, 1, NULL, 'Morning Tea', '2025-05-17', '11:00:00', '11:15:00', 'break'),
(1089, 1, NULL, 'Lunch', '2025-05-17', '12:00:00', '13:00:00', 'break'),
(1090, 1, NULL, 'Afternoon Tea', '2025-05-17', '15:00:00', '15:15:00', 'break'),
(1091, 1, NULL, 'Morning Tea', '2025-05-19', '11:00:00', '11:15:00', 'break'),
(1092, 1, NULL, 'Lunch', '2025-05-19', '12:00:00', '13:00:00', 'break'),
(1093, 1, NULL, 'Afternoon Tea', '2025-05-19', '15:00:00', '15:15:00', 'break'),
(1094, 1, NULL, 'Morning Tea', '2025-05-20', '11:00:00', '11:15:00', 'break'),
(1095, 1, NULL, 'Lunch', '2025-05-20', '12:00:00', '13:00:00', 'break'),
(1096, 1, NULL, 'Afternoon Tea', '2025-05-20', '15:00:00', '15:15:00', 'break'),
(1097, 1, NULL, 'Morning Tea', '2025-05-21', '11:00:00', '11:15:00', 'break'),
(1098, 1, NULL, 'Lunch', '2025-05-21', '12:00:00', '13:00:00', 'break'),
(1099, 1, NULL, 'Afternoon Tea', '2025-05-21', '15:00:00', '15:15:00', 'break');

-- --------------------------------------------------------

--
-- Table structure for table `tasks`
--

CREATE TABLE `tasks` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `type` enum('task','event') COLLATE utf8_unicode_ci DEFAULT 'task',
  `title` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `description` text COLLATE utf8_unicode_ci,
  `category_id` int(11) DEFAULT NULL,
  `sub_category_id` int(11) DEFAULT NULL,
  `time_hrs` int(11) DEFAULT '0',
  `time_mins` int(11) DEFAULT '0',
  `priority` int(11) DEFAULT '3',
  `importance` enum('important','not_important') COLLATE utf8_unicode_ci DEFAULT 'not_important',
  `urgency` enum('urgent','not_urgent') COLLATE utf8_unicode_ci DEFAULT 'not_urgent',
  `due_date` datetime DEFAULT NULL,
  `time_split` enum('yes','no') COLLATE utf8_unicode_ci DEFAULT 'yes',
  `breaks` enum('yes','no') COLLATE utf8_unicode_ci DEFAULT 'yes',
  `start_time` time DEFAULT NULL,
  `end_time` time DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `scheduled` tinyint(1) DEFAULT '0',
  `date` date DEFAULT NULL,
  `completed` tinyint(1) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `tasks`
--

INSERT INTO `tasks` (`id`, `user_id`, `type`, `title`, `description`, `category_id`, `sub_category_id`, `time_hrs`, `time_mins`, `priority`, `importance`, `urgency`, `due_date`, `time_split`, `breaks`, `start_time`, `end_time`, `created_at`, `scheduled`, `date`, `completed`) VALUES
(93, 1, 'task', 'Meeting Prep', NULL, 7, NULL, 0, 15, 1, 'important', 'urgent', '2025-04-23 00:00:00', 'no', 'yes', NULL, NULL, '2025-04-18 10:32:04', 1, NULL, 1),
(94, 1, 'task', 'Test AutoPA', NULL, 7, NULL, 0, 25, 2, 'important', 'not_urgent', '2025-04-22 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-18 10:32:04', 1, NULL, 0),
(96, 1, 'task', 'Do Ironing', NULL, 2, NULL, 0, 45, 4, 'not_important', 'not_urgent', NULL, 'no', 'yes', NULL, NULL, '2025-04-18 10:32:04', 1, NULL, 0),
(97, 1, 'task', 'Vacuum House', NULL, 7, NULL, 1, 20, 5, 'not_important', 'not_urgent', '0000-00-00 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-18 10:32:04', 0, NULL, 0),
(98, 1, 'task', 'Paint Bedroom', NULL, 7, NULL, 2, 0, 3, 'not_important', 'not_urgent', '2025-04-27 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-18 10:32:04', 1, NULL, 0),
(102, 1, 'task', 'Install Roof', NULL, 7, NULL, 3, 45, 2, 'not_important', 'not_urgent', '2025-04-24 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-18 10:34:46', 1, NULL, 0),
(121, 1, 'task', 'Team Sync', NULL, 1, 5, 1, 0, 3, 'not_important', 'not_urgent', '2025-04-25 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-20 23:07:46', 1, NULL, 0),
(122, 6, 'task', 'Wriet up Will', 'Use Carols Template to create my will', 13, NULL, 2, 0, 2, 'important', 'urgent', '2025-05-09 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-21 02:53:07', 0, NULL, 0),
(123, 6, 'task', 'Get Will signed off', 'organise for 2 witnesses', 13, 20, 1, 0, 2, 'important', 'urgent', '2025-05-19 00:00:00', 'yes', 'yes', NULL, NULL, '2025-04-21 03:25:34', 0, NULL, 0);

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `uid` int(11) NOT NULL,
  `username` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `password` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`uid`, `username`, `password`, `created_at`) VALUES
(1, 'testuser', '$2y$10$OgjtHxvsv76rhBYB4dYl2uy.03GalUPEYI5BfKQbYvctBmo1dgy0m', '2025-04-14 22:25:54'),
(2, '', '$2y$10$EJsd3iRhZoWOV.xmZiVnZed0gpQ1LiyNQJ1TtkZvAu6/gledoMwZ6', '2025-04-14 22:52:37'),
(3, 'newuser', '$2y$10$R5H7ubWal1diAi8rfv3gH.8gClPSzoXBBesbe134zrVUJDthwTzW6', '2025-04-14 22:52:55'),
(6, 'Richard', '$2y$10$v15iMUYVOlZGpUAZjTntdefJebhs5Fhg0/vTZAbP6uitIDZeY3gYm', '2025-04-18 22:41:47');

-- --------------------------------------------------------

--
-- Table structure for table `working_hours`
--

CREATE TABLE `working_hours` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `day` enum('Mon','Tue','Wed','Thu','Fri','Sat','Sun') COLLATE utf8_unicode_ci DEFAULT NULL,
  `start_time` time DEFAULT NULL,
  `end_time` time DEFAULT NULL,
  `monday_start` time DEFAULT '08:30:00',
  `monday_end` time DEFAULT '17:00:00',
  `tuesday_start` time DEFAULT '08:30:00',
  `tuesday_end` time DEFAULT '17:00:00',
  `wednesday_start` time DEFAULT '08:30:00',
  `wednesday_end` time DEFAULT '17:00:00',
  `thursday_start` time DEFAULT '08:30:00',
  `thursday_end` time DEFAULT '17:00:00',
  `friday_start` time DEFAULT '08:30:00',
  `friday_end` time DEFAULT '17:00:00',
  `saturday_start` time DEFAULT '09:30:00',
  `saturday_end` time DEFAULT '17:00:00',
  `sunday_start` time DEFAULT '09:30:00',
  `sunday_end` time DEFAULT '17:00:00'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `working_hours`
--

INSERT INTO `working_hours` (`id`, `user_id`, `day`, `start_time`, `end_time`, `monday_start`, `monday_end`, `tuesday_start`, `tuesday_end`, `wednesday_start`, `wednesday_end`, `thursday_start`, `thursday_end`, `friday_start`, `friday_end`, `saturday_start`, `saturday_end`, `sunday_start`, `sunday_end`) VALUES
(78, 1, 'Mon', '09:20:00', '16:30:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(79, 1, 'Tue', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(80, 1, 'Wed', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(81, 1, 'Thu', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(82, 1, 'Fri', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(83, 1, 'Sat', '09:30:00', '12:30:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(84, 6, 'Mon', '08:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(85, 6, 'Tue', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(86, 6, 'Wed', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(87, 6, 'Thu', '09:00:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(88, 6, 'Fri', '09:00:00', '16:30:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(89, 6, 'Sat', '09:30:00', '12:30:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00'),
(90, 6, 'Sun', '00:00:00', '00:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '08:30:00', '17:00:00', '09:30:00', '17:00:00', '09:30:00', '17:00:00');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `breaks`
--
ALTER TABLE `breaks`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`),
  ADD KEY `parent_id` (`parent_id`);

--
-- Indexes for table `events`
--
ALTER TABLE `events`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`);

--
-- Indexes for table `preferences`
--
ALTER TABLE `preferences`
  ADD PRIMARY KEY (`user_id`);

--
-- Indexes for table `schedules`
--
ALTER TABLE `schedules`
  ADD PRIMARY KEY (`id`),
  ADD KEY `task_id` (`task_id`),
  ADD KEY `idx_user_date` (`user_id`,`date`);

--
-- Indexes for table `tasks`
--
ALTER TABLE `tasks`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`),
  ADD KEY `sub_category_id` (`sub_category_id`),
  ADD KEY `tasks_ibfk_2` (`category_id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`uid`),
  ADD UNIQUE KEY `username` (`username`);

--
-- Indexes for table `working_hours`
--
ALTER TABLE `working_hours`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `breaks`
--
ALTER TABLE `breaks`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=56;

--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;

--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=32;

--
-- AUTO_INCREMENT for table `schedules`
--
ALTER TABLE `schedules`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1100;

--
-- AUTO_INCREMENT for table `tasks`
--
ALTER TABLE `tasks`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=124;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `uid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `working_hours`
--
ALTER TABLE `working_hours`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=91;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `breaks`
--
ALTER TABLE `breaks`
  ADD CONSTRAINT `breaks_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`);

--
-- Constraints for table `categories`
--
ALTER TABLE `categories`
  ADD CONSTRAINT `categories_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`),
  ADD CONSTRAINT `categories_ibfk_2` FOREIGN KEY (`parent_id`) REFERENCES `categories` (`id`);

--
-- Constraints for table `events`
--
ALTER TABLE `events`
  ADD CONSTRAINT `events_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`);

--
-- Constraints for table `preferences`
--
ALTER TABLE `preferences`
  ADD CONSTRAINT `preferences_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`);

--
-- Constraints for table `schedules`
--
ALTER TABLE `schedules`
  ADD CONSTRAINT `schedules_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`),
  ADD CONSTRAINT `schedules_ibfk_2` FOREIGN KEY (`task_id`) REFERENCES `tasks` (`id`);

--
-- Constraints for table `tasks`
--
ALTER TABLE `tasks`
  ADD CONSTRAINT `tasks_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`),
  ADD CONSTRAINT `tasks_ibfk_2` FOREIGN KEY (`category_id`) REFERENCES `categories` (`id`) ON DELETE SET NULL,
  ADD CONSTRAINT `tasks_ibfk_3` FOREIGN KEY (`sub_category_id`) REFERENCES `categories` (`id`);

--
-- Constraints for table `working_hours`
--
ALTER TABLE `working_hours`
  ADD CONSTRAINT `working_hours_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`uid`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
