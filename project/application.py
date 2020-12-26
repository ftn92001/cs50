import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Create database
# open("data.db", "w").close()

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///data.db")


@app.route("/")
@login_required
def index():
    rows = db.execute("SELECT * FROM users WHERE id = :id",
                      id=session["user_id"])

    prows = db.execute("SELECT * FROM posts WHERE userid = :id ORDER BY postid DESC",
                      id=session["user_id"])

    return render_template("index.html", rows=rows, prows=prows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("name"):
            return apology("must provide name", 403)

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password(again)", 403)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) != 0:
            return apology("username already exists")

        db.execute("INSERT INTO users (username, hash, name) VALUES (?,?,?)",
                    request.form.get("username"), generate_password_hash(request.form.get("password")), request.form.get("name"))

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash('Registered!')

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/create", methods=["GET", "POST"])
@login_required
def create():
    if request.method == "POST":
        db.execute("INSERT INTO posts (userid, `text-content`) VALUES (?,?)",
                    session["user_id"], request.form.get("post-text"))
        return redirect("/")
    else:
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                      id=session["user_id"])
        return render_template("create.html", rows=rows)

@app.route("/edit", methods=["GET", "POST"])
@login_required
def edit():
    if request.method == "POST":
        db.execute("UPDATE posts SET `text-content`=:text_content, timestamp=current_timestamp WHERE postid = :postid",
                    text_content=request.form.get("post-text"), postid=request.form.get("post-id"))
        return redirect("/")
    else:
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                      id=session["user_id"])
        prows = db.execute("SELECT * FROM posts WHERE userid = :id",
                      id=session["user_id"])
        return render_template("update.html", rows=rows, prows=prows, postid=request.args.get("p"))

@app.route("/delete")
@login_required
def delete():
    db.execute("DELETE FROM posts WHERE postid = :postid",
                    postid=request.args.get("p"))
    return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
