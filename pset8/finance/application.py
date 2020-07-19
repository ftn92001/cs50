import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows = db.execute("SELECT symbol, name, SUM(shares) AS 'shares_sum', price, SUM(total) AS 'total_sum' FROM purchase WHERE user_id = :user_id GROUP BY symbol", user_id=session["user_id"])
    cashs = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])

    total = 0
    for row in rows:
        row["price"] = usd(row["price"])
        total += row["total_sum"]
        row["total_sum"] = usd(row["total_sum"])
    total += cashs[0]["cash"]

    return render_template("index.html", rows=rows, cash=usd(cashs[0]["cash"]), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        info = lookup(request.form.get("symbol"))

        if not info:
            return apology("invalid symbol")

        total = float(request.form.get("shares")) * info["price"]

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        if total > rows[0]["cash"]:
            return apology("can't afford")

        else:
            db.execute("INSERT INTO purchase (user_id, symbol, name, shares, price, total) VALUES (?,?,?,?,?,?)",
                        session["user_id"], info["symbol"], info["name"], request.form.get("shares"), info["price"], total)

        cash = rows[0]["cash"] - total

        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])


        flash('Bought!')

        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT symbol, shares, price, transacted FROM purchase WHERE user_id = :user_id", user_id=session["user_id"])
    return render_template("history.html", rows=rows)


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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        info = lookup(request.form.get("symbol"))

        if not info:
            return apology("invalid symbol")

        return render_template("quoted.html", name=info["name"], price=info["price"], symbol=info["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

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

        db.execute("INSERT INTO users (username, hash) VALUES (?,?)",
                    request.form.get("username"), generate_password_hash(request.form.get("password")))

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


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        info = lookup(request.form.get("symbol"))

        if not info:
            return apology("invalid symbol")

        rows = db.execute("SELECT SUM(shares) AS 'shares_sum' FROM purchase WHERE user_id = :user_id GROUP BY symbol HAVING symbol = :symbol", user_id=session["user_id"], symbol=info["symbol"])

        if int(request.form.get("shares")) > rows[0]["shares_sum"]:
            return apology("too many shares")

        else:
            total = -float(request.form.get("shares")) * info["price"]
            shares = -int(request.form.get("shares"))
            db.execute("INSERT INTO purchase (user_id, symbol, name, shares, price, total) VALUES (?,?,?,?,?,?)",
                        session["user_id"], info["symbol"], info["name"], shares, info["price"], total)

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        cash = rows[0]["cash"] - total

        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])


        flash('Sold!')

        return redirect("/")

    else:
        rows = db.execute("SELECT symbol FROM purchase WHERE user_id = :user_id GROUP BY symbol", user_id=session["user_id"])
        return render_template("sell.html", rows=rows)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
