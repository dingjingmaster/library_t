<?php
/**
 * Created by PhpStorm.
 * User: dingjing
 * Date: 2019/03/26
 * Time: 14:48
 */

spl_autoload_register(function ($className) {
    require_once __DIR__ . '/../' . $className . '.php';
});

$whoops = new Whoops\Run();
//$whoops->pushHandler(new Whoops\Handler\JsonResponseHandler());
$whoops->pushHandler(new \Whoops\Handler\PlainTextHandler());
$whoops->register();

//var_dump($_GET)

/* 以上配置则完成了 whoops 的错误处理 */

$a(); // 随手写的一个 错误
?>