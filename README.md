# How to use

## Available routes

- `POST` `/api/users/register` - pass `body = {email: "<email>", password: "<password>"}`
- `POST` `/api/users/login` - pass `body = {email: "<email>", password: "<password>"}`
- `PATCH` `/api/users` - pass `jwt-token` as `Authorization` header, `body = {password: "<password>", newPassword:"<newPassword>"}`
- `DELETE` `/api/users?id=<id>` - pass `jwt-token` as `Authorization` header, `body = {password: "<password>", newPassword:"<newPassword>"}`
- `GET` `/api/tasks?userId=<userId>` - pass `jwt-token` as `Authorization` header
- `POST` `/api/tasks?userId=<userId>` - pass `jwt-token` as `Authorization` header, `body = {data: "<todo-note>"}`
- `PATCH` `/api/tasks?userId=<userId>&id=<taskId>` - pass `jwt-token` as `Authorization` header, `body = {data: "<new-todo-note>"}`
- `DELETE` `/api/tasks?userId=<userId>&id=<taskId>` - pass `jwt-token` as `Authorization` header
